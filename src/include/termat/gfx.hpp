/// path: include/termat/gfx
//
#pragma once
//
/// Resource imports
#include <windows.h>
#include <string>
//
/// NameSpace Rendering manager
namespace termat::gfx {

    enum COLOUR {
        FG_BLACK		= 0x0000,
        FG_DARK_BLUE    = 0x0001,	
        FG_DARK_GREEN   = 0x0002,
        FG_DARK_CYAN    = 0x0003,
        FG_DARK_RED     = 0x0004,
        FG_DARK_MAGENTA = 0x0005,
        FG_DARK_YELLOW  = 0x0006,
        FG_GREY			= 0x0007,
        FG_DARK_GREY    = 0x0008,
        FG_BLUE			= 0x0009,
        FG_GREEN		= 0x000A,
        FG_CYAN			= 0x000B,
        FG_RED			= 0x000C,
        FG_MAGENTA		= 0x000D,
        FG_YELLOW		= 0x000E,
        FG_WHITE		= 0x000F,
        BG_BLACK		= 0x0000,
        BG_DARK_BLUE	= 0x0010,
        BG_DARK_GREEN	= 0x0020,
        BG_DARK_CYAN	= 0x0030,
        BG_DARK_RED		= 0x0040,
        BG_DARK_MAGENTA = 0x0050,
        BG_DARK_YELLOW	= 0x0060,
        BG_GREY			= 0x0070,
        BG_DARK_GREY	= 0x0080,
        BG_BLUE			= 0x0090,
        BG_GREEN		= 0x00A0,
        BG_CYAN			= 0x00B0,
        BG_RED			= 0x00C0,
        BG_MAGENTA		= 0x00D0,
        BG_YELLOW		= 0x00E0,
        BG_WHITE		= 0x00F0,
    };

    enum PIXEL_TYPE {
        PIXEL_SOLID = 0x2588,
        PIXEL_THREEQUARTERS = 0x2593,
        PIXEL_HALF = 0x2592,
        PIXEL_QUARTER = 0x2591,
    };

    namespace Font {
        // Fonte Tiny5 (ASCII 32–90)
        static const uint8_t Tiny5[][5] = {
            { 0x00, 0x00, 0x00, 0x00, 0x00 }, // SPACE (32)
            { 0x04, 0x04, 0x04, 0x00, 0x04 }, // !     (33)
            { 0x0A, 0x0A, 0x00, 0x00, 0x00 }, // "     (34)
            { 0x0A, 0x1F, 0x0A, 0x1F, 0x0A }, // #     (35) 
            { 0x0F, 0x14, 0x0E, 0x05, 0x1E }, // $     (36)
            { 0x09, 0x02, 0x04, 0x09, 0x00 }, // %     (37)
            { 0x0C, 0x10, 0x0D, 0x12, 0x0D }, // &     (38)
            { 0x04, 0x04, 0x00, 0x00, 0x00 }, // '     (39)
            { 0x02, 0x04, 0x04, 0x04, 0x02 }, // (     (40)
            { 0x08, 0x04, 0x04, 0x04, 0x08 }, // )     (41)
            { 0x00, 0x0A, 0x04, 0x0A, 0x00 }, // *     (42)
            { 0x00, 0x04, 0x0E, 0x04, 0x00 }, // +     (43)
            { 0x00, 0x00, 0x00, 0x04, 0x08 }, // ,     (44)
            { 0x00, 0x00, 0x0E, 0x00, 0x00 }, // -     (45)
            { 0x00, 0x00, 0x00, 0x00, 0x04 }, // .     (46)
            { 0x02, 0x02, 0x04, 0x08, 0x08 }, // /     (47)
            { 0x0E, 0x11, 0x11, 0x11, 0x0E }, // 0     (48)
            { 0x04, 0x0C, 0x04, 0x04, 0x0E }, // 1     (49)
            { 0x0E, 0x11, 0x02, 0x04, 0x1F }, // 2     (50)
            { 0x1F, 0x01, 0x0E, 0x01, 0x1F }, // 3     (51)
            { 0x02, 0x06, 0x0A, 0x1F, 0x02 }, // 4     (52)
            { 0x1F, 0x10, 0x1E, 0x01, 0x1E }, // 5     (53)
            { 0x06, 0x08, 0x1E, 0x11, 0x0E }, // 6     (54)
            { 0x1F, 0x01, 0x02, 0x04, 0x04 }, // 7     (55)
            { 0x0E, 0x11, 0x0E, 0x11, 0x0E }, // 8     (56)
            { 0x0E, 0x11, 0x0F, 0x01, 0x06 }, // 9     (57)
            { 0x04, 0x00, 0x00, 0x04, 0x00 }, // :     (58)
            { 0x04, 0x00, 0x00, 0x04, 0x08 }, // ;     (59)
            { 0x04, 0x08, 0x10, 0x08, 0x04 }, // <     (60)
            { 0x00, 0x1F, 0x00, 0x1F, 0x00 }, // =     (61)
            { 0x04, 0x02, 0x01, 0x02, 0x04 }, // >     (62)
            { 0x0E, 0x02, 0x04, 0x00, 0x04 }, // ?     (63)
            { 0x0E, 0x11, 0x15, 0x19, 0x0E }, // @     (64)
            { 0x0E, 0x11, 0x11, 0x1F, 0x11 }, // A     (65)
            { 0x1E, 0x11, 0x1E, 0x11, 0x1E }, // B     (66)
            { 0x0E, 0x11, 0x10, 0x11, 0x0E }, // C     (67)
            { 0x1E, 0x11, 0x11, 0x11, 0x1E }, // D     (68)
            { 0x1F, 0x10, 0x1E, 0x10, 0x1F }, // E     (69)
            { 0x1F, 0x10, 0x1E, 0x10, 0x10 }, // F     (70)
            { 0x0E, 0x11, 0x11, 0x15, 0x0E }, // G     (71)
            { 0x11, 0x11, 0x1F, 0x11, 0x11 }, // H     (72)
            { 0x0E, 0x04, 0x04, 0x04, 0x0E }, // I     (73)
            { 0x07, 0x02, 0x02, 0x12, 0x0C }, // J     (74)
            { 0x11, 0x12, 0x1C, 0x12, 0x11 }, // K     (75)
            { 0x10, 0x10, 0x10, 0x10, 0x1F }, // L     (76)
            { 0x11, 0x1B, 0x15, 0x11, 0x11 }, // M     (77)
            { 0x11, 0x19, 0x15, 0x13, 0x11 }, // N     (78)
            { 0x0E, 0x11, 0x11, 0x11, 0x0E }, // O     (79)
            { 0x1E, 0x11, 0x1E, 0x10, 0x10 }, // P     (80)
            { 0x0E, 0x11, 0x11, 0x0E, 0x04 }, // Q     (81)
            { 0x1E, 0x11, 0x1E, 0x12, 0x11 }, // R     (82)
            { 0x0F, 0x10, 0x0E, 0x01, 0x1E }, // S     (83)
            { 0x1F, 0x04, 0x04, 0x04, 0x04 }, // T     (84)
            { 0x11, 0x11, 0x11, 0x11, 0x0E }, // U     (85)
            { 0x11, 0x11, 0x11, 0x0A, 0x04 }, // V     (86)
            { 0x11, 0x11, 0x15, 0x1B, 0x11 }, // W     (87)
            { 0x11, 0x0A, 0x04, 0x0A, 0x11 }, // X     (88)
            { 0x11, 0x11, 0x0A, 0x04, 0x04 }, // Y     (89)
            { 0x1F, 0x02, 0x04, 0x08, 0x1F }  // Z     (90)
        };
    }
    
    // Main system
    class Renderer {
    protected:
        CHAR_INFO* m_targetBuffer = nullptr;
        int m_width = 0, m_height = 0;

    public:
        virtual ~Renderer() = default;

        void linkTarget(CHAR_INFO* buffer, int w, int h) {
            m_targetBuffer = buffer;
            m_width = w; m_height = h;
        }

        // Função atômica usada por todos os tipos (2D e 3D)
        void Pixel(int x, int y, short c = 0x2588, short col = 0x000F) {
            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                m_targetBuffer[y * m_width + x].Char.UnicodeChar = c;
                m_targetBuffer[y * m_width + x].Attributes = col;
            }
        }
    };

    // 2D Sub-system
    class Renderer2D : public Renderer {
    protected:
    public:
        void String(int x, int y, std::wstring c, short col = 0x000F) {
            if (!m_targetBuffer) return;
            for (size_t i = 0; i < c.size(); i++) {
               int px = x + (int)i;
                if (px < 0 || px >= m_width || y < 0 || y >= m_height) continue;
                m_targetBuffer[y * m_width + px].Char.UnicodeChar = c[i];
                m_targetBuffer[y * m_width + px].Attributes = col;
            }
        }

        void Clip(int &x, int &y) {
            if (x < 0) x = 0;
            if (x >= m_width) x = m_width - 1;
            if (y < 0) y = 0;
            if (y >= m_height) y = m_height - 1;
        }

        void Line(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F) {
            int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
            dx = x2 - x1; dy = y2 - y1;
            dx1 = abs(dx); dy1 = abs(dy);
            px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
            if (dy1 <= dx1) {
                if (dx >= 0) { x = x1; y = y1; xe = x2; }
                else { x = x2; y = y2; xe = x1;}

                Pixel(x, y, c, col);
                
                for (i = 0; x<xe; i++) {
                    x = x + 1;
                    if (px<0) px = px + 2 * dy1;
                    else {
                        if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
                        px = px + 2 * (dy1 - dx1);
                    }
                    Pixel(x, y, c, col);
                }
            }
            else {
                if (dy >= 0) { x = x1; y = y1; ye = y2; }
                else { x = x2; y = y2; ye = y1; }

                Pixel(x, y, c, col);

                for (i = 0; y<ye; i++) {
                    y = y + 1;
                    if (py <= 0) py = py + 2 * dx1;
                    else {
                        if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
                        py = py + 2 * (dx1 - dy1);
                    }
                    Pixel(x, y, c, col);
                }
            }
        }

        void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F) {
            Line(x1, y1, x2, y2, c, col);
            Line(x2, y2, x3, y3, c, col);
            Line(x3, y3, x1, y1, c, col);
        }

        // https://www.avrfreaks.net/sites/default/files/triangles.c
        void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F) {
            auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
            auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Pixel(i, ny, c, col); };
            
            int t1x, t2x, y, minx, maxx, t1xp, t2xp;
            bool changed1 = false;
            bool changed2 = false;
            int signx1, signx2, dx1, dy1, dx2, dy2;
            int e1, e2;
            // Sort vertices
            if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
            if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
            if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

            t1x = t2x = x1; y = y1;   // Starting points
            dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
            else signx1 = 1;
            dy1 = (int)(y2 - y1);

            dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
            else signx2 = 1;
            dy2 = (int)(y3 - y1);

            if (dy1 > dx1) {   // swap values
                SWAP(dx1, dy1);
                changed1 = true;
            }
            if (dy2 > dx2) {   // swap values
                SWAP(dy2, dx2);
                changed2 = true;
            }

            e2 = (int)(dx2 >> 1);
            // Flat top, just process the second half
            if (y1 == y2) goto next;
            e1 = (int)(dx1 >> 1);

            for (int i = 0; i < dx1;) {
                t1xp = 0; t2xp = 0;
                if (t1x<t2x) { minx = t1x; maxx = t2x; }
                else { minx = t2x; maxx = t1x; }
                // process first line until y value is about to change
                while (i<dx1) {
                    i++;
                    e1 += dy1;
                    while (e1 >= dx1) {
                        e1 -= dx1;
                        if (changed1) t1xp = signx1;//t1x += signx1;
                        else          goto next1;
                    }
                    if (changed1) break;
                    else t1x += signx1;
                }
                // Move line
            next1:
                // process second line until y value is about to change
                while (1) {
                    e2 += dy2;
                    while (e2 >= dx2) {
                        e2 -= dx2;
                        if (changed2) t2xp = signx2;//t2x += signx2;
                        else          goto next2;
                    }
                    if (changed2)     break;
                    else              t2x += signx2;
                }
            next2:
                if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
                if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
                drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                            // Now increase y
                if (!changed1) t1x += signx1;
                t1x += t1xp;
                if (!changed2) t2x += signx2;
                t2x += t2xp;
                y += 1;
                if (y == y2) break;

            }
        next:
            // Second half
            dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
            else signx1 = 1;
            dy1 = (int)(y3 - y2);
            t1x = x2;

            if (dy1 > dx1) {   // swap values
                SWAP(dy1, dx1);
                changed1 = true;
            }
            else changed1 = false;

            e1 = (int)(dx1 >> 1);

            for (int i = 0; i <= dx1; i++) {
                t1xp = 0; t2xp = 0;
                if (t1x<t2x) { minx = t1x; maxx = t2x; }
                else { minx = t2x; maxx = t1x; }
                // process first line until y value is about to change
                while (i<dx1) {
                    e1 += dy1;
                    while (e1 >= dx1) {
                        e1 -= dx1;
                        if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                        else          goto next3;
                    }
                    if (changed1) break;
                    else   	   	  t1x += signx1;
                    if (i<dx1) i++;
                }
            next3:
                // process second line until y value is about to change
                while (t2x != x3) {
                    e2 += dy2;
                    while (e2 >= dx2) {
                        e2 -= dx2;
                        if (changed2) t2xp = signx2;
                        else          goto next4;
                    }
                    if (changed2)     break;
                    else              t2x += signx2;
                }
            next4:

                if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
                if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
                drawline(minx, maxx, y);   										
                if (!changed1) t1x += signx1;
                t1x += t1xp;
                if (!changed2) t2x += signx2;
                t2x += t2xp;
                y += 1;
                if (y>y3) return;
            }
        }

        void Rect(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F) {
            Line(x1, y1, x2 - 1, y1, c, col);           // Top
            Line(x1, y2 - 1, x2 - 1, y2 - 1, c, col);   // Bottom
            Line(x1, y1, x1, y2 - 1, c, col);           // Left
            Line(x2 - 1, y1, x2 - 1, y2 - 1, c, col);   // Right
        }

        void FillRect(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F) {
            for (int x = x1; x < x2; x++) for (int y = y1; y < y2; y++) Pixel(x, y, c, col);
        }


        void Circle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F) {
            int x = 0;
            int y = r;
            int p = 3 - 2 * r;
            if (!r) return;

            while (y >= x) // only formulate 1/8 of circle
            {
                Pixel(xc - x, yc - y, c, col);//upper left left
                Pixel(xc - y, yc - x, c, col);//upper upper left
                Pixel(xc + y, yc - x, c, col);//upper upper right
                Pixel(xc + x, yc - y, c, col);//upper right right
                Pixel(xc - x, yc + y, c, col);//lower left left
                Pixel(xc - y, yc + x, c, col);//lower lower left
                Pixel(xc + y, yc + x, c, col);//lower lower right
                Pixel(xc + x, yc + y, c, col);//lower right right
                if (p < 0) p += 4 * x++ + 6;
                else p += 4 * (x++ - y--) + 10;
            }
        }

        void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F) {
            // Taken from wikipedia
            int x = 0;
            int y = r;
            int p = 3 - 2 * r;
            if (!r) return;

            auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Pixel(i, ny, c, col); };

            while (y >= x) {
                // Modified to draw scan-lines instead of edges
                drawline(xc - x, xc + x, yc - y);
                drawline(xc - y, xc + y, yc - x);
                drawline(xc - x, xc + x, yc + y);
                drawline(xc - y, xc + y, yc + x);
                if (p < 0) p += 4 * x++ + 6;
                else p += 4 * (x++ - y--) + 10;
            }
        };

        void Char(int x, int y, wchar_t ch, int scale = 1, int spacing = 1, short c = 0x2588, short col = 0x000F) {
            if (!m_targetBuffer) return;

            if (ch < 32 || ch > 90) return;

            const uint8_t* glyph = Font::Tiny5[ch - 32];

            for (int gy = 0; gy < 5; gy++) {
                for (int gx = 0; gx < 5; gx++) {
                    if (!(glyph[gx] & (1 << gy))) continue;
                    for (int sy = 0; sy < scale; sy++) {
                        for (int sx = 0; sx < scale; sx++) Pixel(x + gx * scale + sx, y + gy * scale + sy, c, col);
                    }
                }
            }
        }

        void Text(int x, int y, const std::wstring& text, int scale = 1, int spacing = 1, short pixel = 0x2588, short col = 0x000F) {
            int cx = x;

            for (wchar_t ch : text) {
                if (ch == L'\n') {
                    cx = x;
                    y += (5 * scale) + spacing;
                    continue;
                }

                Char(cx, y, ch, scale, spacing, pixel, col);
                cx += (5 * scale) + spacing;
            }
        }
    };

    // 3D Sub-system
    class Renderer3D : public Renderer {
        protected:
        public:
    };
}