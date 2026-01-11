/// path: include/termat/gfx
//
#pragma once
//
/// Resource imports
#include <windows.h>
#include <string>
#include <vector>
#include <array>
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
    
    // Main system
    class Renderer {
    protected:
        CHAR_INFO* m_targetBuffer = nullptr;
        int m_width = 0, m_height = 0;

    public:
        virtual ~Renderer() = default;

        void linkTarget(CHAR_INFO* buffer, int w, int h);
        void Pixel(int x, int y, short c = 0x2588, short col = 0x000F); // Função atômica (2D e 3D)
    };

    // 2D Sub-system
    class Renderer2D : public Renderer {
    protected:
    public:
        // Utils
        void Clip(int &x, int &y);

        // 
        void String(int x, int y, std::wstring c, short col = 0x000F);

        // Geometric Drawners
        void Line(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);
        void Rect(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void FillRect(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
        void Circle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);
        void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F);
        void Polygon(const std::vector<std::array<int, 2>>& points, short c = 0x2588, short col = 0x000F);
        void FillPolygon(const std::vector<std::pair<int, int>>& pts, short c = 0x2588, short col = 0x000F);

        // Fonts Drawners
        void Char(int x, int y, wchar_t ch, int scale = 1, int spacing = 1, short c = 0x2588, short col = 0x000F);
        void Text(int x, int y, const std::wstring& text, int scale = 1, int spacing = 1, short c = 0x2588, short col = 0x000F);

        // Sprite Drawners
        void ImagePPM(const std::wstring& filename, int ox, int oy, int maxW = 128, int maxH = 128);
    };

    // 3D Sub-system
    class Renderer3D : public Renderer {
        protected:
        public:
    };
}