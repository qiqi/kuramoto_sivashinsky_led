#include<math.h>
#include<string.h>
#include <FastLED.h>
#define DATA_PIN 6

template<unsigned int _n_seg>
class CRGBMap
{
    protected:
        CRGB _color[_n_seg+1];

        void _define_segment(int i_seg,
                             uint32_t r, uint32_t g, uint32_t b)
        {
            _color[i_seg][0] = r;
            _color[i_seg][1] = g;
            _color[i_seg][2] = b;
        }

    public:
        void float2rgb(float v, CRGB& color)
        {
            if (v > 1) v = 1;
            if (v < -1) v = -1;
            int i_seg = (int)((v + 1) / 2 * _n_seg);
            if (i_seg == _n_seg) --i_seg;
            float frac = (v + 1) / 2 * _n_seg - i_seg;
            uint8_t rgb[3];
            for (int i = 0; i < 3; ++i)
            {
                int c = (int)round((1 - frac) * _color[i_seg][i]
                                 + frac * _color[i_seg + 1][i]);
                if (c < 0) c = 0;
                if (c > 255) c = 255;
                rgb[i] = c;
            }
            color = CRGB(rgb[0], rgb[1], rgb[2]);
            /*
            Serial.println(v);
            Serial.println(i_seg);
            Serial.println(frac);
            Serial.println(_color[i_seg][2]);
            Serial.println(_color[i_seg+1][2]);
            Serial.println(rgb[0]);
            Serial.println(rgb[1]);
            Serial.println(rgb[2]);
            Serial.println("RGB");
            */
            // color = CRGB(0, 200, 0);
        }
};

class SeismicCRGBMap : public CRGBMap<10>
{
    public:
    SeismicCRGBMap()
    {
        _define_segment(0,   20, 255, 255);
        _define_segment(1,  100, 250, 100);
        _define_segment(2,  250, 150,   0);
        _define_segment(3,  255,   0,   0);
        _define_segment(4,  150,   0,   0);
        _define_segment(5,    5,   5,   5);
        _define_segment(6,    0, 150,   0);
        _define_segment(7,    0, 255, 100);
        _define_segment(8,    0, 255, 255);
        _define_segment(9,   50, 255, 150);
        _define_segment(10, 150, 255, 255);
    }
};

float timestep(float stencil[5])
{
    const float dx = 0.8;
    const float dx2 = dx * dx;
    const float dt = dx2 * dx2 / 8;
    float lapl = (stencil[1] + stencil[3] - 2 * stencil[2]) / dx2;
    float lapl_p = (stencil[2] + stencil[4] - 2 * stencil[3]) / dx2;
    float lapl_m = (stencil[0] + stencil[2] - 2 * stencil[1]) / dx2;
    float lapl2 = (lapl_p + lapl_m - 2 * lapl) / dx2;
    float u_dudx = (stencil[2] - 0.3) * (stencil[3] - stencil[1]) / (2 * dx);
    float dudt = 3 * u_dudx - lapl - lapl2;
    return stencil[2] + dudt * dt;
}

const int N = 50;
float val[N+1];
float stencil[5] = {0, 0, 0, 1.0, 0.0};
SeismicCRGBMap cm;
CRGB colors[3*N];

void val2color(float val[], int N)
{
    for (int i = 0; i < N; ++i)
    {
        float v0 = val[i];
        float v1 = (2 * val[i] + val[i+1]) / 3;
        float v2 = (val[i] + 2 * val[i+1]) / 3;
        cm.float2rgb(v0, colors[i*3]);
        cm.float2rgb(v1, colors[i*3+1]);
        cm.float2rgb(v2, colors[i*3+2]);
    }
    FastLED.show(); 
}

void setup() {
    pinMode(DATA_PIN, OUTPUT);
    memset(val, 0, sizeof(float) * (N+1));
    //for (int i = 0; i <=N; i += 10) val[i] = 1.0;
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(colors, 3*N);
    Serial.begin(9600);
}

void loop() {
    //for (int i = 0; i < 150; ++i) colors[i] = CRGB::Red; FastLED.show(); delay(500); 
    //for (int i = 0; i < 150; ++i) colors[i] = CRGB::Black; FastLED.show(); delay(500);
    static int i_step = 0;
    for (int i_bunch = 0; i_bunch < 10; ++i_bunch)
    {
      ++i_step;
      Serial.println(i_step);
      stencil[1] = 0.0;
      stencil[2] = 0.0;
      for (int i = 0; i <= N; ++i)
      {
          memmove(stencil, stencil+1, sizeof(float) * 4);
          stencil[4] = (i + 2 <= N) ? val[i+2] : 0.0;
          val[i] = timestep(stencil);
      }
    }
    val2color(val, N);
}
