#include<iostream>
#include "colormap.h"

float timestep(float stencil[5])
{
    const float dx = 0.25;
    const float dx2 = dx * dx;
    const float dt = dx2 * dx2 / 16;
    float lapl = (stencil[1] + stencil[3] - 2 * stencil[2]) / dx2;
    float lapl_p = (stencil[2] + stencil[4] - 2 * stencil[3]) / dx2;
    float lapl_m = (stencil[0] + stencil[2] - 2 * stencil[1]) / dx2;
    float lapl2 = (lapl_p + lapl_m - 2 * lapl) / dx2;
    float u_dudx = stencil[2] * (stencil[3] - stencil[1]) / (2 * dx);
    float dudt = u_dudx - lapl - lapl2;
    return stencil[2] + dudt * dt;
}

int main()
{
    const int N = 1000;
    SeismicColorMap cm;
    Color colors[N];
    float val[N];
    for (int i = 0; i < N; ++i)
    {
        cm.float2rgb(0.0, colors[i]);
        val[i] = 0.0;
    }

    float stencil[5] = {0.0, -1, 1.5, 0.2, 0.0};
    for (int steps = 0; steps < 100; ++steps)
    {
        for (int i = 0; i < N; ++i)
        {
            memmove(stencil, stencil+1, sizeof(float) * 4);
            int ip2 = (i + 2) % N;
            stencil[4] = cm.rgb2float(colors[ip2]);
            stencil[4] = val[ip2];
            cm.float2rgb(timestep(stencil), colors[i]);
            val[i] = timestep(stencil);
            std::cout << cm.rgb2float(colors[i]) << ":";
            std::cout << val[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
