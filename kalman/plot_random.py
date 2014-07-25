#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# room6933.com/blog/2011/08/03/multivariate_normal_python/ から抜粋
#
# pythonで二次元正規分布に従う乱数をプロット
#
 
import numpy as np
import matplotlib.pyplot as pylab
 
#平均
mu1 = [-2,-2]
mu2 = [2,2]
#共分散
cov = [[2,1],[1,2]]

#500はデータ数
x1,y1 = np.random.multivariate_normal(mu1,cov,500).T
x2,y2 = np.random.multivariate_normal(mu2,cov,500).T

#グラフ描画
#背景を白にする
pylab.figure(facecolor="w")

#散布図をプロットする
pylab.scatter(x1,y1,color='r',marker='x',label="$K_1,mu_1$")
pylab.scatter(x2,y2,color='b',marker='x',label="$K_2,mu_2$")

#ついでにグラフの中に文字を入れてみる
pylab.figtext(0.8,0.6,"$R_1$",size=20)
pylab.figtext(0.2,0.3,"$R_2$",size=20)

#ラベル
pylab.xlabel('$x$',size=20)
pylab.ylabel('$y$',size=20)

#軸
pylab.axis([-10.0,10.0,-10.0,10.0],size=20)
pylab.grid(True)
pylab.legend()

#保存
pylab.savefig("multivariate_normal.png",format = 'png', dpi=200)
pylab.show()
pylab.close()
