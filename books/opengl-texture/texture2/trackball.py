#!/usr/bin/env python
# -*- coding: utf-8 -*-

from math import *

# ドラッグ開始位置
cx, cy = 0, 0

# マウスの絶対位置→ウィンドウ内での相対位置の換算係
sx, sy = 0.0, 0.0

#マウスの相対位置→回転角の換算係数 */
SCALE = 2.0 * 3.14159265358979323846

# 回転の初期値 (クォータニオン)
cq = [ 1.0, 0.0, 0.0, 0.0 ]

# ドラッグ中の回転 (クォータニオン)
tq = [ 1.0, 0.0, 0.0, 0.0 ]

#
rt = [
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0,
]

#ドラッグ中か否か
drag = 0

def trackballInit():
    global drag
    drag = 0
    cq = [ 1.0, 0.0, 0.0, 0.0 ]

def trackballRegion(w, h):
    global sx, sy
    # マウスポインタ位置のウィンドウ内の相対的位置への換算用
    sx = 1.0 / w
    sy = 1.0 / h

def trackballStart(x, y):
    global drag, cx, cy
    # ドラッグ開始
    drag = 1

    # ドラッグ開始点を記録
    cx = x
    cy = y

def trackballMotion(x, y):
    global drag, cx, cy, sx, sy, cq, tq, rt
    if drag == 1:
        # マウスポインタの位置のドラッグ開始位置からの変位
        dx = (x - cx) * sx
        dy = (y - cy) * sy
        
        # マウスポインタの位置のドラッグ開始位置からの距離
        a = sqrt(dx * dx + dy * dy)

        if a != 0.0:
            ar = a * SCALE * 0.5
            as0 = sin(ar) / a
            dq = [ cos(ar), dy * as0, dx * as0, 0.0 ]
            
            # クォータニオンを掛けて回転を合成
            tq = qmul(dq, cq)
      
            # クォータニオンから回転の変換行列を求める
            rt = qrot(tq)


def trackballStop(x, y):
    global drag, cq, tq
    # ドラッグ終了点における回転を求める
    trackballMotion(x, y)

    # 回転の保存
    cq[0] = tq[0]
    cq[1] = tq[1]
    cq[2] = tq[2]
    cq[3] = tq[3]

    # ドラッグ終了
    drag = 0


def trackballRotation():
    global rt
    return [rt[0:4], rt[4:8], rt[8:12], rt[12:16]]

def qmul(p, q):
    r = [ 0.0, 0.0, 0.0, 0.0 ]
    r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3]
    r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2]
    r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1]
    r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0]
    return r

def qrot(q):
    r = [ 1.0, 0.0, 0.0, 0.0,
          0.0, 1.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0 ]

    x2 = q[1] * q[1] * 2.0
    y2 = q[2] * q[2] * 2.0
    z2 = q[3] * q[3] * 2.0
    xy = q[1] * q[2] * 2.0
    yz = q[2] * q[3] * 2.0
    zx = q[3] * q[1] * 2.0
    xw = q[1] * q[0] * 2.0
    yw = q[2] * q[0] * 2.0
    zw = q[3] * q[0] * 2.0
  
    r[ 0] = 1.0 - y2 - z2
    r[ 1] = xy + zw
    r[ 2] = zx - yw
    r[ 4] = xy - zw
    r[ 5] = 1.0 - z2 - x2
    r[ 6] = yz + xw
    r[ 8] = zx + yw
    r[ 9] = yz - xw
    r[10] = 1.0 - x2 - y2
    r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0
    r[15] = 1.0
    return r
