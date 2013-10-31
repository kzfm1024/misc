#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import array

def sphere_vertex_array(radius, slices, stacks):
    vertices = []
    for st in range(stacks + 1):
        ph = math.pi * st / stacks
        y = float(radius * math.cos(ph))
        r = float(radius * math.sin(ph))
        for sl in range(slices + 1):
            th = 2.0 * math.pi * sl / slices
            x = float(r * math.cos(th))
            z = float(r * math.sin(th))
            vertices.append(x)
            vertices.append(y)
            vertices.append(z)
    return array.array('f', vertices)


def sphere_index_array(slices, stacks):
    indices = []
    for st in range(stacks):
        for sl in range(slices):
            count = (slices + 1) * st + sl
            #
            indices.append(count)
            indices.append(count + 1)
            indices.append(count + slices + 2)
            #
            indices.append(count)
            indices.append(count + slices + 1)
            indices.append(count + slices + 2)
    return array.array('I', indices)


def sphere_texcoord_array(slices, stacks):
    uv = []
    for st in range(stacks + 1):
        for sl in range(slices + 1):
            u = float(sl) / float(slices)
            v = float(st) / float(stacks)
            uv.append(u)
            uv.append(v)
            uv.append(1.0)
    return array.array('f', uv)


if __name__ == '__main__':
    print sphere_vertex_array(1, 4, 2)
    print sphere_index_array(4, 2)
    print sphere_texcoord_array(4, 2)
    print len(sphere_index_array(4, 2))
