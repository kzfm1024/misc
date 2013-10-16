#!/usr/bin/env python
# -*- coding: utf-8 -*-

# http://d.hatena.ne.jp/norio515/20111112/1321105385
# juliaset.py

import pycuda.autoinit
import pycuda.driver as drv
import numpy as np
from pycuda.compiler import SourceModule

mod = SourceModule("""
struct cuComplex {
    float r;
    float i;
    __device__ cuComplex(float a, float b) : r(a), i(b) {}
    __device__ float magnitude2(void) {
        return r * r + i * i;
    }
    __device__ cuComplex operator*(const cuComplex& a) {
        return cuComplex(r*a.r - i*a.i, i*a.r + r*a.i);
    }
    __device__ cuComplex operator+(const cuComplex& a) {
        return cuComplex(r+a.r, i+a.i);
    }
};

__device__ int julia(int x, int y, int w, int h)
{
    const float scale = 1.5;
    float jx = scale * (float)(w / 2 - x) / (w / 2);
    float jy = scale * (float)(h / 2 - y) / (h / 2);
    cuComplex c(-0.8, 0.156);
    cuComplex a(jx, jy);
    int i = 0;
    for (i = 0; i < 200; i++) {
        a = a * a + c;
        if (a.magnitude2() > 1000) {
            return 0;
        }
    }
    return 1;
}

__global__ void kernel(int *ptr)
{
    int x = blockIdx.x;
    int y = blockIdx.y;
    int w = gridDim.x;
    int h = gridDim.y;
    int i = 4 * (x + y * w);

    int jval = julia(x, y, w, h);
    ptr[i] = 255 * jval;
    ptr[i + 1] = 0;
    ptr[i + 2] = 0;
    ptr[i + 3] = 255;
}
""")

def julia_by_cpu(data, size):
    w, h = size
    def julia(x, y):
        scale = 1.5
        jx = scale * (w / 2.0 - x) / (w / 2.0)
        jy = scale * (h / 2.0 - y) / (h / 2.0)
        c = complex(-0.8, 0.156)
        a = complex(jx, jy)
        magnitude2 = lambda comp: comp.real**2 + comp.imag**2  
        for i in xrange(200):
            a = a * a + c
            if magnitude2(a) > 1000:
                return 0
        return 1
            
    for x in xrange(w):
        for y in xrange(h):
            i = 4 * (x + y * w)
            data[i] = 255 * julia(x, y)
            data[i + 1] = 0
            data[i + 2] = 0
            data[i + 3] = 255

class Bitmap(object):
    def __init__(self, size):
        self.width, self.height = size
        self.data = np.zeros(self.width * self.height * 4, np.int32)

    def display(self):
        import sys
        import pygame as pg
        pg.init()
        scr = pg.display.set_mode((self.width, self.height))
        pg.display.set_caption("Julia Set")
        surface = pg.Surface((self.width, self.height))
        data = self.data
        for x in xrange(self.width):
            for y in xrange(self.height):
                i = 4 * (x + y * self.width)
                color = pg.Color(int(data[i]),
                                 int(data[i+1]),
                                 int(data[i+2]),
                                 int(data[i+3]))
                surface.set_at((x, y), color)
        while True:
            scr.blit(surface, (0, 0))
            pg.display.update()
            for e in pg.event.get():
                if e.type == pg.QUIT:
                    sys.exit()

def speedtest(func, *args, **kw):
    start = drv.Event()
    end = drv.Event()
    start.record()
    func(*args, **kw)
    end.record()
    end.synchronize()
    secs = start.time_till(end)*1e-3
    print("%s: %fs" % (func, secs))

if __name__ == "__main__":
    scr_size = (640, 480)
    bitmap = Bitmap(scr_size)
    def gpu_code():
        d_bitmap = drv.mem_alloc(bitmap.data.nbytes)
        drv.memcpy_htod(d_bitmap, bitmap.data)
        kernel = mod.get_function("kernel")
        speedtest(kernel, d_bitmap, block=(1, 1, 1), grid=scr_size)
        drv.memcpy_dtoh(bitmap.data, d_bitmap)
        
    def cpu_code():
        speedtest(julia_by_cpu, bitmap.data, scr_size)

    import sys
    if len(sys.argv) > 1 and sys.argv[1] == "cpu":
        cpu_code()
    else:
        gpu_code()
    bitmap.display()
