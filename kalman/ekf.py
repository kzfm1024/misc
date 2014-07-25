#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# satomacoto.blogspot.jp/2011/06/python_22.html から抜粋
#

import numpy as np
import scipy as sp
from scipy.optimize.slsqp import approx_jacobian
import matplotlib.pyplot as plt
 
def main():
    # 初期化
    T = 50 # 観測数
    p0 = (0,0); p1 = (10,0); p2 = (0,10) # 観測値の座標
    x = np.mat([[0],[0]]) # 初期位置
    X = [np.mat([[0],[0]])] # 状態
    Y = [np.mat([[0],[0],[0]])] # 観測
 
    # state x = A * x_ + B * u + w, w~N(0,Q)
    A = np.mat([[1,0],[0,1]])
    B = np.mat([[1,0],[0,1]])
    u = np.mat([[2],[2]])
    Q = np.mat([[1,0],[0,1]])
    # observation Y = h(x) + v, v~N(0,R)
    R = np.mat([[2,0,0],[0,2,0],[0,0,2]])
    def h_(x,p):
        return ((x[0]-p[0])**2 + (x[1]-p[1])**2)**.5
    def h(x):
        x = (x[0,0],x[1,0])
        return np.mat([[h_(x,p0)],[h_(x,p1)],[h_(x,p2)]])
    def Jh0(x):        
        """
        解析的に求めるh(x)のヤコビアン
        """
        x = (x[0,0],x[1,0])
        return np.mat([[(x[0]-p0[0])/h_(x,p0),(x[1]-p0[1])/h_(x,p0)],
                       [(x[0]-p1[0])/h_(x,p1),(x[1]-p1[1])/h_(x,p1)],
                       [(x[0]-p2[0])/h_(x,p2),(x[1]-p2[1])/h_(x,p2)]])
    def Jh1(x):
        """
        数値的に求めるh(x)のヤコビアン
        """
        x = (x[0,0],x[1,0])
        h = lambda x: np.asfarray([h_(x,p0),h_(x,p1),h_(x,p2)])
        return np.mat(approx_jacobian(x,h,np.sqrt(np.finfo(float).eps)))        
     
    # 観測データの生成
    for i in range(T):
        x = A * x + B * u + np.random.multivariate_normal([0,0],Q,1).T
        X.append(x)
        y = h(x) + np.random.multivariate_normal([0,0,0],R,1).T
        Y.append(y)
     
    # EKF
    mu = np.mat([[0],[0]])
    Sigma = np.mat([[0,0],[0,0]])    
    M = [mu] # 推定
    for i in range(T):
        # prediction
        mu_ = A * mu + B * u
        Sigma_ = Q + A * Sigma * A.T
        # update
        #C = Jh0(mu_) # 解析的
        C = Jh1(mu_) # 数値的
        yi = Y[i+1] - C * mu_
        S = C * Sigma_ * C.T + R
        K = Sigma_ * C.T * S.I
        mu = mu_ + K * yi
        Sigma = Sigma_ - K * C * Sigma_
        M.append(mu)
 
    # 描画
    a,b = np.array(np.concatenate(X,axis=1))
    plt.plot(a,b,'rs-')
    a,b = np.array(np.concatenate(M,axis=1))
    plt.plot(a,b,'bo-')
    plt.axis('equal')
    plt.show()
 
if __name__ == '__main__':
    main()
