#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import gym
import numpy as np
import time

class CartPoleQLearner:

    def __init__(self, ):
        self.alpha = 0.2 # 学習率
        self.gamma = 0.99 # 時間割引き率

        self.env = gym.make('CartPole-v0')
        self.qtable = np.zeros((50, 50, 50, 50, 2))
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

    def state(self, observation):
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

    def action(self, observation, epsilon):
        if np.random.uniform(0, 1) >= epsilon:
            cart_p, cart_v, pole_p, pole_v = self.state(observation)
            return np.argmax(self.qtable[cart_p][cart_v][pole_p][pole_v])
        else:
            return np.random.choice([0, 1])

    def save_qtable(self, path):
        np.save(path, self.qtable)

    def load_qtable(self, path):
        self.q_talble = np.load(file=path)

    def update_qtable(self, action, observation, next_observation, reward):
        # 行動後の状態で得られる最大行動価値 Q(s',a')
        next_cart_p, next_cart_v, next_pole_p, next_pole_v = self.state(next_observation)
        next_max_q_value = max(self.qtable[next_cart_p][next_cart_v][next_pole_p][next_pole_v])

        # 行動前の状態の行動価値 Q(s,a)
        cart_p, cart_v, pole_p, pole_v = self.state(observation)        
        q_value = self.qtable[cart_p][cart_v][pole_p][pole_v][action]

        # 行動価値関数の更新
        self.qtable[cart_p][cart_v][pole_p][pole_v][action] = q_value + self.alpha * (reward + self.gamma * next_max_q_value - q_value)

    def learn(self, num_episodes, num_timesteps, *, epsilon):
        rewards = []
        for ep in range(num_episodes):
            observation = self.env.reset()
            total_reward = 0
            for t in range(num_timesteps):
                action = self.action(observation, epsilon)
                next_observation, reward, done, _ = self.env.step(action)
                self.update_qtable(action, observation, next_observation, reward)
                total_reward += reward
                observation = next_observation
                if done:
                    print('episode {0} timessteps {1} total_reward {2}'.format(ep+1, t+1, total_reward))
                    rewards.append(total_reward)
                    break
        return rewards

    def run(self, num_episodes, num_timesteps, *, epsilon):
        rewards = []
        for ep in range(num_episodes):
            observation = self.env.reset()
            total_reward = 0
            for t in range(num_timesteps):
                self.env.render()
                time.sleep(0.033)
                action = self.action(observation, epsilon)
                next_observation, reward, done, _ = self.env.step(action)
                total_reward += reward
                observation = next_observation
            print('episode {0} timessteps {1} total_reward {2}'.format(ep+1, num_timesteps, total_reward))
    
def main():
    cartpole = CartPoleQLearner()
    rewards = cartpole.learn(50000, 200, epsilon=0.25)
    cartpole.save_qtable('cartpole_qtable_ep50000')
    #np.save('cartpole_rewards_ep50000', rewards)
    rewards = cartpole.run(100, 200, epsilon=1.0) # random action
    rewards = cartpole.run(100, 200, epsilon=0.0) # learned action 

if __name__ == '__main__':
    main()
