import numpy as np
import matplotlib.pyplot as plt

def average_100episodes(rewards):
    rewards_100episodes = rewards.reshape(500,100)
    return np.array([np.average(i) for i in rewards_100episodes])

def main():
    rewards1 = np.load(file='rewards-learn-epsilon0.1.npy')
    rewards2 = np.load(file='rewards-learn-epsilon0.2.npy')
    rewards3 = np.load(file='rewards-learn-epsilon0.3.npy')
    rewards4 = np.load(file='rewards-learn-epsilon0.4.npy')
    rewards5 = np.load(file='rewards-learn-epsilon0.5.npy')
    average1 = average_100episodes(rewards1)
    average2 = average_100episodes(rewards2)
    average3 = average_100episodes(rewards3)
    average4 = average_100episodes(rewards4)
    average5 = average_100episodes(rewards5)

    index = np.array([0, 10000, 20000, 30000, 40000, 50000])
    
    plt.xlabel('average reward of each 100 episodes')
    plt.ylabel('rewards')    
    plt.ylim(0, 200)
    
    plt.plot(average1, label='epsilon=0.1')
    plt.plot(average2, label='epsilon=0.2')
    plt.plot(average3, label='epsilon=0.3')
    plt.plot(average4, label='epsilon=0.4')
    plt.plot(average5, label='epsilon=0.5')
    plt.legend(loc="upper left")
    plt.show()

if __name__ == '__main__':
    main()
