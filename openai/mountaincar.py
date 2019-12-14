#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import gym
import numpy as np
import time

class MountainCar:

    def __init__(self, ):
        self.env = gym.make('MountainCar-v0')
        self.q_table = np.zeros((40, 40, 3))
        
    def __del__(self):
        self.env.close()

    def get_status(self, _observation):
        env_low = self.env.observation_space.low # 位置と速度の最小値
        env_high = self.env.observation_space.high #　位置と速度の最大値
        env_dx = (env_high - env_low) / 40 # 40等分
        # 0〜39の離散値に変換する
        position = int((_observation[0] - env_low[0])/env_dx[0])
        velocity = int((_observation[1] - env_low[1])/env_dx[1])
        return position, velocity
        
    def get_action(self, _observation):
        epsilon = 0.002
        if np.random.uniform(0, 1) > epsilon:
            position, velocity = self.get_status(_observation)
            action = np.argmax(self.q_table[position][velocity])
        else:
            action = np.random.choice([0, 1, 2])
        return action

    def update_q_table(self, _action,  _observation, _next_observation, _reward):
        alpha = 0.2 # 学習率
        gamma = 0.99 # 時間割引き率

        # 行動後の状態で得られる最大行動価値 Q(s',a')
        next_position, next_velocity = self.get_status(_next_observation)
        next_max_q_value = max(self.q_table[next_position][next_velocity])

        # 行動前の状態の行動価値 Q(s,a)
        position, velocity = self.get_status(_observation)
        q_value = self.q_table[position][velocity][_action]

        # 行動価値関数の更新
        self.q_table[position][velocity][_action] = q_value + alpha * (_reward + gamma * next_max_q_value - q_value)

        
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
                    print('episode {0} total_reward {1}'.format(ep, total_reward))
                    rewards.append(total_reward)
                    break
        return rewards

    def simulate_random_action(self, num_episodes, num_timesteps):
        for ep in range(num_episodes):
            observation = self.env.reset()
            rewards = []
            for t in range(num_timesteps):
                self.env.render()
                time.sleep(0.033)
                action = self.env.action_space.sample()
                next_observation, reward, done, _ = self.env.step(action)
                rewards.append(reward)
            print('Episode {0} total reward {1}'.format(ep+1, sum(rewards)))
        
def main():
    mountaincar = MountainCar()
    #mountaincar.simulate_random_action(10, 100)
    mountaincar.learn(10000, 200)

if __name__ == '__main__':
    main()
