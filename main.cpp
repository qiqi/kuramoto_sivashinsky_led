#include<iostream>
#include "colormap.h"

float timestep(float stencil[5])
{
    const float dx = 0.5;
    const float dx2 = dx * dx;
    const float dt = dx2 * dx2 / 8;
    float lapl = (stencil[1] + stencil[3] - 2 * stencil[2]) / dx2;
    float lapl_p = (stencil[2] + stencil[4] - 2 * stencil[3]) / dx2;
    float lapl_m = (stencil[0] + stencil[2] - 2 * stencil[1]) / dx2;
    float lapl2 = (lapl_p + lapl_m - 2 * lapl) / dx2;
    float u_dudx = stencil[2] * (stencil[3] - stencil[1]) / (2 * dx);
    float dudt = 3 * u_dudx - lapl - lapl2;
    return stencil[2] + dudt * dt;
}

void val2color(float val[], int N)
{
    static SeismicColorMap cm;
    Color colors[3*N];
    for (int i = 0; i < N; ++i)
    {
        float v0 = val[i];
        float v1 = (2 * val[i] + val[i+1]) / 3;
        float v2 = (val[i] + 2 * val[i+1]) / 3;
        cm.float2rgb(v0, colors[i*3]);
        cm.float2rgb(v1, colors[i*3+1]);
        cm.float2rgb(v2, colors[i*3+2]);
    }
    static int steps = 0;
    steps ++;
    if (steps % 100 == 0) {
    for (int i = 0; i < 3*N; ++i)
        std::cout << int(colors[i][0]) << " "
                  << int(colors[i][1]) << " "
                  << int(colors[i][2]) << " ";
    std::cout << std::endl;
    }
}

int main()
{
    const int N = 100;
    float val[N+1];
    memset(val, 0, sizeof(float) * (N+1));

    float stencil[5] = {0, 0, 0, 1.0, 0.0};
    for (int steps = 0; steps < 100000; ++steps)
    {
        stencil[1] = 0.0;
        stencil[2] = 0.0;
        for (int i = 0; i <= N; ++i)
        {
            memmove(stencil, stencil+1, sizeof(float) * 4);
            stencil[4] = (i + 2 <= N) ? val[i+2] : 0.0;
            val[i] = timestep(stencil);
        }
        val2color(val, N);
    }
    return 0;
}
