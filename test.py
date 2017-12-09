from pylab import *
from numpy import *

u = zeros(300, float32)
u[0] = 1
for steps in range(100000):
    dx = 0.5
    dx2 = dx**2
    dt = dx2**2 / 16
    z = zeros(1,float32)
    uu = hstack([z, u, z])
    up = hstack([u, z, z])
    um = hstack([z, z, u])
    lapl = (up + um - 2*uu) / dx2
    lapl2 = (lapl[2:] + lapl[:-2] - 2*lapl[1:-1]) / dx2
    u_dudx = u * (up[1:-1] - um[1:-1]) / (2*dx)
    dudt = 3 * u_dudx - lapl[1:-1] - lapl2
    u += dudt * dt
    if steps % 1000 == 0:
        plot(u)
