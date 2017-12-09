#include<math.h>
#include<stdint.h>
#include<string.h>
#include<assert.h>

typedef uint8_t Color[3];

class ColorMap
{
    protected:
        int _n_seg;
        Color * _color;

        void _define_segment(int i_seg,
                             uint32_t r, uint32_t g, uint32_t b)
        {
            _color[i_seg][0] = r;
            _color[i_seg][1] = g;
            _color[i_seg][2] = b;
        }

    public:
        ColorMap(int n) : _n_seg(n)
        {
            _color = new Color[n+1];
        }

        ~ColorMap()
        {
            delete _color;
        }

        void float2rgb(float v, Color color)
        {
            if (v > 1) v = 1;
            if (v < -1) v = -1;
            int i_seg = (int)((v + 1) / 2 * _n_seg);
            assert(i_seg >= 0);
            assert(i_seg <= _n_seg);
            if (i_seg == _n_seg) --i_seg;
            float frac = (v + 1) / 2 * _n_seg - i_seg;
            for (int i = 0; i < 3; ++i)
            {
                int c = (int)round((1 - frac) * _color[i_seg][i]
                                 + frac * _color[i_seg + 1][i]);
                if (c < 0) c = 0;
                if (c > 255) c = 255;
                color[i] = c;
            }
        }
};

class SeismicColorMap : public ColorMap
{
    public:
    SeismicColorMap() : ColorMap(11)
    {
        _define_segment(0,  255, 255, 240);
        _define_segment(1,  255, 250, 100);
        _define_segment(2,  255, 150,   0);
        _define_segment(3,  255,   0,   0);
        _define_segment(4,  150,   0,   0);
        _define_segment(5,    0,   0,   0);
        _define_segment(6,    0,   0, 150);
        _define_segment(7,    0,   0, 255);
        _define_segment(8,    0, 150, 255);
        _define_segment(9,    0, 250, 255);
        _define_segment(10, 100, 250, 255);
        _define_segment(11, 240, 255, 255);
    }
};
