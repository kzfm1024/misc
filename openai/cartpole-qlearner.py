#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import gym
import numpy as np
import time

class CartPoleQLearner:

    def __init__(self, *, debug=False):
        self.gamma = 0.9 # 割引率
        self.alpha = 0.1 # 学習率

        self.env = gym.make('CartPole-v0')

        #
        # 連続値である状態を (50,50,50,50) 次元の状態に離散化して Q 学習
        #
        self.qtable = np.zeros((50, 50, 50, 50, 2))
        
        #
        # 車体の速度は [-2.0, 2.0] に範囲にクリッピング
        # 棒の先端の速度は [-3.5, 3.5] に範囲にクリッピング        
        #
        self.obs_min = self.env.observation_space.low
        self.obs_min[1] = -2.0 # cart velocity 
        self.obs_min[3] = -3.5 # pole velocity at tip
        
        self.obs_max = self.env.observation_space.high
        self.obs_max[1] = 2.0 # cart velocity 
        self.obs_max[3] = 3.5 # pole velocity at tip

        self.obs_delta = (self.obs_max - self.obs_min) / 50

        if debug:
            print('obs_min   ' + str(self.obs_min))
            print('obs_max   ' + str(self.obs_max))
            print('obs_delta ' + str(self.obs_delta))

    def __del__(self):
        self.env.close()

    def clip(self, val):
        ival = int(val)
        if ival < 0:
            return 0
        elif ival >= 50:
            return 49
        else:
            return ival

    def state(self, observation):
        obs = observation
        cart_p = self.clip((obs[0] - self.obs_min[0]) / self.obs_delta[0])
        cart_v = self.clip((obs[1] - self.obs_min[1]) / self.obs_delta[1])
        pole_p = self.clip((obs[2] - self.obs_min[2]) / self.obs_delta[2])
        pole_v = self.clip((obs[3] - self.obs_min[3]) / self.obs_delta[3])
        return cart_p, cart_v, pole_p, pole_v

    def action(self, observation, epsilon):
        if np.random.uniform(0, 1) >= epsilon:
            cart_p, cart_v, pole_p, pole_v = self.state(observation)
            return np.argmax(self.qtable[cart_p][cart_v][pole_p][pole_v])
        else:
            return np.random.choice([0, 1])

    def reset_qtable(self):
        self.qtable = np.zeros((50, 50, 50, 50, 2))

    def save_qtable(self, path):
        np.save(path, self.qtable)

    def load_qtable(self, path):
        self.qtable = np.load(file=path)

    def update_qtable(self, action, observation, next_observation, reward):
        # 行動後の状態で得られる最大行動価値 Q(s',a')
        next_cart_p, next_cart_v, next_pole_p, next_pole_v = self.state(next_observation)
        next_max_qvalue = max(self.qtable[next_cart_p][next_cart_v][next_pole_p][next_pole_v])

        # 行動前の状態の行動価値 Q(s,a)
        cart_p, cart_v, pole_p, pole_v = self.state(observation)        
        qvalue = self.qtable[cart_p][cart_v][pole_p][pole_v][action]

        # 行動価値関数の更新
        self.qtable[cart_p][cart_v][pole_p][pole_v][action] = qvalue + self.alpha * (reward + self.gamma * next_max_qvalue - qvalue)

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
                    rewards.append(total_reward)
                    break
        return np.array(rewards)

    def run(self, num_episodes, num_timesteps, *, epsilon=0.0, render=False):
        rewards = []
        for ep in range(num_episodes):
            observation = self.env.reset()
            total_reward = 0
            for t in range(num_timesteps):
                if render:
                    self.env.render()
                    time.sleep(0.033)
                action = self.action(observation, epsilon)
                next_observation, reward, done, _ = self.env.step(action)
                total_reward += reward
                observation = next_observation
            rewards.append(total_reward)
        return np.array(rewards)

if __name__ == '__main__':
    learner = CartPoleQLearner()
    rewards = learner.learn(50000, 200, epsilon=0.1)
    learner.save_qtable('qtable-learn-epsilon0.1')
    np.save('rewards-learn-epsilon0.1', rewards)

    learner.reset_qtable()
    rewards = learner.learn(50000, 200, epsilon=0.2)
    learner.save_qtable('qtable-learn-epsilon0.2')
    np.save('rewards-learn-epsilon0.2', rewards)

    learner.reset_qtable()
    rewards = learner.learn(50000, 200, epsilon=0.3)
    learner.save_qtable('qtable-learn-epsilon0.3')
    np.save('rewards-learn-epsilon0.3', rewards)

    learner.reset_qtable()
    rewards = learner.learn(50000, 200, epsilon=0.4)
    learner.save_qtable('qtable-learn-epsilon0.4')
    np.save('rewards-learn-epsilon0.4', rewards)

    learner.reset_qtable()
    rewards = learner.learn(50000, 200, epsilon=0.5)
    learner.save_qtable('qtable-learn-epsilon0.5')
    np.save('rewards-learn-epsilon0.5', rewards)

    runner = CartPoleQLearner()
    runner.load_qtable('qtable-learn-epsilon0.1.npy')
    rewards = runner.run(100, 200)
    np.save('rewards-run-epsilon0.1', rewards)
    print('average of rewards (epsilon=0.1) {0}'.format(np.average(rewards)))

    runner.load_qtable('qtable-learn-epsilon0.2.npy')
    rewards = runner.run(100, 200)
    np.save('rewards-run-epsilon0.2', rewards)
    print('average of rewards (epsilon=0.2) {0}'.format(np.average(rewards)))

    runner.load_qtable('qtable-learn-epsilon0.3.npy')
    rewards = runner.run(100, 200)
    np.save('rewards-run-epsilon0.3', rewards)
    print('average of rewards (epsilon=0.3) {0}'.format(np.average(rewards)))

    runner.load_qtable('qtable-learn-epsilon0.4.npy')
    rewards = runner.run(100, 200)
    np.save('rewards-run-epsilon0.4', rewards)
    print('average of rewards (epsilon=0.4) {0}'.format(np.average(rewards)))

    runner.load_qtable('qtable-learn-epsilon0.5.npy')
    rewards = runner.run(100, 200)
    np.save('rewards-run-epsilon0.5', rewards)
    print('average of rewards (epsilon=0.5) {0}'.format(np.average(rewards)))

    #runner.run(100, 10, epsilon=1.0, render=True) # random action
    #runner.run(100, 10, epsilon=0.0, render=True) # learned action
