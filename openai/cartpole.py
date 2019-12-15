#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import gym
import numpy as np
import time

class CartPole:

    def __init__(self, ):
        self.epsilon = 0.25


        self.env = gym.make('CartPole-v0')
        self.q_table = np.zeros((50, 50, 50, 50, 2))
        # print(self.env.observation_space.low)
        # [-4.8000002e+00 -3.4028235e+38 -4.1887903e-01 -3.4028235e+38]
        # print(self.env.observation_space.high)
        # [4.8000002e+00 3.4028235e+38 4.1887903e-01 3.4028235e+38]
        
        self.observation_min = self.env.observation_space.low
        self.observation_min[1] = -2.0 # cart velocity 
        self.observation_min[3] = -3.5 # pole velocity at tip
        print(self.observation_min)
        
        self.observation_max = self.env.observation_space.high
        self.observation_max[1] = 2.0 # cart velocity 
        self.observation_max[3] = 3.5 # pole velocity at tip
        print(self.observation_max)

        self.observation_delta = (self.observation_max - self.observation_min) / 50
        print(self.observation_delta)

    def __del__(self):
        self.env.close()

    def get_status(self, observation):
        obs = observation
        if obs[1] < self.observation_min[1]:
            obs[1] = self.observation_min[1]
        if obs[1] > self.observation_max[1]:
            obs[1] = self.observation_max[1]

        if obs[3] < self.observation_min[3]:
            obs[3] = self.observation_min[3]
        if obs[3] > self.observation_max[3]:
            obs[3] = self.observation_max[3]

        cart_p = int((obs[0] - self.observation_min[0]) / self.observation_delta[0])
        cart_v = int((obs[1] - self.observation_min[1]) / self.observation_delta[1])
        pole_p = int((obs[2] - self.observation_min[2]) / self.observation_delta[2])
        pole_v = int((obs[3] - self.observation_min[3]) / self.observation_delta[3])
        return cart_p, cart_v, pole_p, pole_v

    def get_action(self, observation):
        #epsilon = 0.01
        epsilon = 0.25
        if np.random.uniform(0, 1) > epsilon:
            cart_p, cart_v, pole_p, pole_v = self.get_status(observation)
            action = np.argmax(self.q_table[cart_p][cart_v][pole_p][pole_v])
        else:
            action = np.random.choice([0, 1])
        return action

    def update_q_table(self, action, observation, next_observation, reward):
        alpha = 0.2 # 学習率
        gamma = 0.99 # 時間割引き率

        # 行動後の状態で得られる最大行動価値 Q(s',a')
        next_cart_p, next_cart_v, next_pole_p, next_pole_v = self.get_status(next_observation)
        next_max_q_value = max(self.q_table[next_cart_p][next_cart_v][next_pole_p][next_pole_v])

        # 行動前の状態の行動価値 Q(s,a)
        cart_p, cart_v, pole_p, pole_v = self.get_status(observation)        
        q_value = self.q_table[cart_p][cart_v][pole_p][pole_v][action]

        # 行動価値関数の更新
        self.q_table[cart_p][cart_v][pole_p][pole_v][action] = q_value + alpha * (reward + gamma * next_max_q_value - q_value)

    def simulate_random_action(self, num_episodes, num_timesteps):
        for ep in range(num_episodes):
            observation = self.env.reset()
            rewards = []
            for t in range(num_timesteps):
                self.env.render()
                time.sleep(0.033)
                action = self.env.action_space.sample()
                next_observation, reward, done, _ = self.env.step(action)
                print(next_observation)
                rewards.append(reward)
            print('Episode {0} total reward {1}'.format(ep+1, sum(rewards)))
        
    def learn(self, num_episodes, num_timesteps):
        rewards = []
        for ep in range(num_episodes):
            observation = self.env.reset()
            total_reward = 0
            for t in range(num_timesteps):
                self.env.render()
                #time.sleep(0.033)
                action = self.get_action(observation)
                next_observation, reward, done, _ = self.env.step(action)
                self.update_q_table(action, observation, next_observation, reward)
                total_reward += reward
                observation = next_observation
                if done:
                    print('episode {0} timessteps {1} total_reward {2}'.format(ep, t, total_reward))
                    #print(next_observation)
                    rewards.append(total_reward)
                    break
        return rewards
    
def main():
    cartpole = CartPole()
    #cartpole.simulate_random_action(10, 200)
    cartpole.learn(50000, 200)

if __name__ == '__main__':
    main()
