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
        int _i_seg[3][3][3];
        int * _seg_to_prim_color;

        void _define_segment(int i_seg,
                             uint32_t r, uint32_t g, uint32_t b,
                             int r_range, int g_range, int b_range,
                             int prim_color)
        {
            _color[i_seg][0] = r;
            _color[i_seg][1] = g;
            _color[i_seg][2] = b;
            if (i_seg < _n_seg) {
                _i_seg[r_range][g_range][b_range] = i_seg;
                _seg_to_prim_color[i_seg] = prim_color;

                if (_color[i_seg][prim_color] == 0) {
                    switch (prim_color)
                    {
                    case 0:
                        _i_seg[0][g_range][b_range] = i_seg;
                        break;
                    case 1:
                        _i_seg[r_range][0][b_range] = i_seg;
                        break;
                    case 2:
                        _i_seg[r_range][g_range][0] = i_seg;
                    }
                }
                else if (_color[i_seg][prim_color] == 255) {
                    switch (prim_color)
                    {
                    case 0:
                        _i_seg[2][g_range][b_range] = i_seg;
                        break;
                    case 1:
                        _i_seg[r_range][2][b_range] = i_seg;
                        break;
                    case 2:
                        _i_seg[r_range][g_range][2] = i_seg;
                    }
                }
            }
        }

    public:
        ColorMap(int n) : _n_seg(n)
        {
            _color = new Color[n+1];
            _seg_to_prim_color = new int[n];
        }

        ~ColorMap()
        {
            delete _color;
            delete _seg_to_prim_color;
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
                color[i] = (int)round((1 - frac) * _color[i_seg][i]
                                    + frac * _color[i_seg + 1][i]);
            }
            int ir = (color[0] > 0) + (color[0] == 255);
            int ig = (color[1] > 0) + (color[1] == 255);
            int ib = (color[2] > 0) + (color[2] == 255);
            assert(i_seg + 1 >= _i_seg[ir][ig][ib]);
            assert(i_seg - 1 <= _i_seg[ir][ig][ib]);
        }

        float rgb2float(Color color)
        {
            int ir = (color[0] > 0) + (color[0] == 255);
            int ig = (color[1] > 0) + (color[1] == 255);
            int ib = (color[2] > 0) + (color[2] == 255);
            int i_seg = _i_seg[ir][ig][ib];
            int i_color = _seg_to_prim_color[i_seg];
            int a = _color[i_seg][i_color];
            int b = _color[i_seg+1][i_color];
            int x = color[i_color];
            float frac = (float)(x - a) / (b - a);
            return (i_seg + frac) / _n_seg * 2 - 1;
        }
};

class SeismicColorMap : public ColorMap
{
    public:
    SeismicColorMap() : ColorMap(4)
    {
        _define_segment(0, 255,   0,  50, 2, 0, 1, 2);
        _define_segment(1, 255,   0, 255, 2, 1, 2, 1);
        _define_segment(2, 255, 255, 255, 2, 2, 1, 2);
        _define_segment(3, 255, 255,   0, 1, 2, 0, 0);
        _define_segment(4,  60, 255,   0, 0, 0, 0, 0);
    }
};
