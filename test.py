from __future__ import division
import matplotlib
matplotlib.interactive(True)
import time
from pylab import *
from numpy import *

u_all = []
u = zeros(50, float32)
u[::10] = 1
for steps in range(100000):
    dx = 0.8
    dx2 = dx**2
    dt = dx2**2 / 8
    z = zeros(1,float32)
    uu = hstack([z, u, z])
    up = hstack([u, z, z])
    um = hstack([z, z, u])
    lapl = (up + um - 2*uu) / dx2
    lapl2 = (lapl[2:] + lapl[:-2] - 2*lapl[1:-1]) / dx2
    u_dudx = (u - 0.5) * (up[1:-1] - um[1:-1]) / (2*dx)
    dudt = u_dudx - lapl[1:-1] - lapl2
    u += array(dudt * dt, float32)
    # u_next = u + around(dudt * dt)
    # u = array(maximum(minimum(u_next, 32760007), -30002767), float32)
    if steps % 10 == 0:
        u_all.append(u.copy())

contourf(u_all, 100)
