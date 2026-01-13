/// path: src/gfx/renderer.cpp
//
/// Resource imports
#include <termat/gfx.hpp>
//
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
//
#ifndef ASSETS_PATH
#error "ASSETS_PATH nao esta definido"
#endif
//
/// NameSpace Rendering manager
namespace termat::gfx {

    namespace Font 
    {
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

    /// ================= Renderer =================
    //
    void Renderer::linkTarget(CHAR_INFO* buffer, int w, int h) {
        m_targetBuffer = buffer;
        m_width = w; m_height = h;
    }
    //
    void Renderer::Pixel(int x, int y, short c, short col) {
        if (!m_targetBuffer) return;
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;

        m_targetBuffer[y * m_width + x].Char.UnicodeChar = c;
        m_targetBuffer[y * m_width + x].Attributes = col;
    }

    /// ================= Renderer2D =================
    //
    void Renderer2D::Clip(int &x, int &y) {
        if (x < 0) x = 0;
        if (x >= m_width) x = m_width - 1;
        if (y < 0) y = 0;
        if (y >= m_height) y = m_height - 1;
    }

    void Renderer2D::String(int x, int y, std::wstring c, short col) {
        if (!m_targetBuffer) return;
        for (size_t i = 0; i < c.size(); i++) {
            int px = x + (int)i;
            if (px < 0 || px >= m_width || y < 0 || y >= m_height) continue;
            m_targetBuffer[y * m_width + px].Char.UnicodeChar = c[i];
            m_targetBuffer[y * m_width + px].Attributes = col;
        }
    }

    void Renderer2D::Line(int x0, int y0, int x1, int y1, short c, short col) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy, e2;

        while (true) {
            // Só desenha se estiver dentro dos limites do buffer
            if (x0 >= 0 && x0 < m_width && y0 >= 0 && y0 < m_height) { // Calculamos o ponteiro apenas se estiver visível
                m_targetBuffer[y0 * m_width + x0].Char.UnicodeChar = c;
                m_targetBuffer[y0 * m_width + x0].Attributes = col;
            }

            if (x0 == x1 && y0 == y1) break;

            e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx) { err += dx; y0 += sy; }
        }
    }

    void Renderer2D::Rect(int x1, int y1, int x2, int y2, short c, short col) {
        // 1. Limites de segurança para os loops (Evita crash)
        int left   = (std::max)(0, x1);
        int right  = (std::min)(m_width - 1, x2 - 1);
        int top    = (std::max)(0, y1);
        int bottom = (std::min)(m_height - 1, y2 - 1);

        // Se o retângulo está totalmente fora da tela, sai fora
        if (left > right || top > bottom) return;

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* buffer = (uint32_t*)m_targetBuffer;

        // --- LINHAS HORIZONTAIS ---
        // Só desenha a borda superior se o Y original estiver na tela
        if (y1 >= 0 && y1 < m_height) {
            uint32_t* p = &buffer[y1 * m_width + left];
            for (int x = left; x <= right; ++x) *p++ = pixelData;
        }

        // Só desenha a borda inferior se o Y original estiver na tela (e for diferente do topo)
        if (y2 - 1 >= 0 && y2 - 1 < m_height && (y2 - 1 != y1)) {
            uint32_t* p = &buffer[(y2 - 1) * m_width + left];
            for (int x = left; x <= right; ++x) *p++ = pixelData;
        }

        // --- LINHAS VERTICAIS ---
        // Só desenha a borda esquerda se o X original estiver na tela
        if (x1 >= 0 && x1 < m_width) {
            uint32_t* p = &buffer[top * m_width + x1];
            // Note: aqui usamos 'top' e 'bottom' clipados para o loop ser seguro
            for (int y = top; y <= bottom; ++y) {
                *p = pixelData;
                p += m_width;
            }
        }

        // Só desenha a borda direita se o X original estiver na tela
        if (x2 - 1 >= 0 && x2 - 1 < m_width && (x2 - 1 != x1)) {
            uint32_t* p = &buffer[top * m_width + (x2 - 1)];
            for (int y = top; y <= bottom; ++y) {
                *p = pixelData;
                p += m_width;
            }
        }
    }

    void Renderer2D::FillRect(int x1, int y1, int x2, int y2, short c, short col) {
        int left = (std::max)(0, x1);
        int right = (std::min)(m_width, x2);
        int top = (std::max)(0, y1);
        int bottom = (std::min)(m_height, y2);

        if (left >= right || top >= bottom) return;

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint64_t doublePixel = ((uint64_t)pixelData << 32) | pixelData; // 2 pixels em 64 bits
        uint32_t* buffer = (uint32_t*)m_targetBuffer;
        int widthToFill = right - left;

        for (int y = top; y < bottom; ++y) {
            uint32_t* pRow = &buffer[y * m_width + left];
            int x = 0;

            // Processa 4 pixels de uma vez usando uint64_t (2 escritas de 64 bits)
            for (; x <= widthToFill - 4; x += 4) {
                *(uint64_t*)(pRow + x) = doublePixel;
                *(uint64_t*)(pRow + x + 2) = doublePixel;
            }

            // Limpa os pixels restantes (caso a largura não seja múltipla de 4)
            for (; x < widthToFill; ++x) pRow[x] = pixelData;
        }
    }

    void Renderer2D::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col) {
        Line(x1, y1, x2, y2, c, col);
        Line(x2, y2, x3, y3, c, col);
        Line(x3, y3, x1, y1, c, col);
    }

    void Renderer2D::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col) {
        // 1. Sort Vertices por Y (y1 <= y2 <= y3)
        if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
        if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
        if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

        if (y1 == y3) return; // Triângulo sem altura

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* buffer = (uint32_t*)m_targetBuffer;

        // Lambda para preencher a linha horizontal de forma ultra-rápida
        auto drawHLine = [&](int xStart, int xEnd, int y) {
            if (y < 0 || y >= m_height) return;
            if (xStart > xEnd) std::swap(xStart, xEnd);
            int left = (std::max)(0, xStart);
            int right = (std::min)(m_width - 1, xEnd);
            if (left <= right) {
                uint32_t* p = &buffer[y * m_width + left];
                // Uso de ponteiro direto: Auge da performance
                for (int i = 0; i <= (right - left); ++i) p[i] = pixelData;
            }
        };

        // 2. Cálculo dos passos (Slopes) usando Ponto Fixo (multiplicado por 1024 para evitar float)
        // Usamos 16.16 bits ou apenas float se a CPU for moderna (2026 floats são rápidos)
        auto getStep = [](int xA, int yA, int xB, int yB) {
            if (yA == yB) return 0.0f;
            return (float)(xB - xA) / (float)(yB - yA);
        };

        float step13 = getStep(x1, y1, x3, y3);
        float step12 = getStep(x1, y1, x2, y2);
        float step23 = getStep(x2, y2, x3, y3);

        float curX1 = (float)x1;
        float curX2 = (float)x1;

        // 3. Parte Superior: do y1 até y2
        for (int y = y1; y < y2; y++) {
            drawHLine((int)curX1, (int)curX2, y);
            curX1 += step13;
            curX2 += step12;
        }

        // 4. Parte Inferior: do y2 até y3
        curX2 = (float)x2; // Reinicia a aresta curta na nova inclinação
        for (int y = y2; y <= y3; y++) {
            drawHLine((int)curX1, (int)curX2, y);
            curX1 += step13;
            curX2 += step23;
        }
    }

    void Renderer2D::Ellipse(int xc, int yc, int rx, int ry, short c, short col) {
        if (rx <= 0 || ry <= 0) return;

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* buffer = (uint32_t*)m_targetBuffer;

        // Lambda rápida para pintar 4 pontos simétricos com checagem de limites
        auto plot4 = [&](int x, int y) {
            int posX = xc + x, negX = xc - x;
            int posY = yc + y, negY = yc - y;

            if (posY >= 0 && posY < m_height) {
                if (posX >= 0 && posX < m_width) buffer[posY * m_width + posX] = pixelData;
                if (negX >= 0 && negX < m_width) buffer[posY * m_width + negX] = pixelData;
            }
            if (negY >= 0 && negY < m_height) {
                if (posX >= 0 && posX < m_width) buffer[negY * m_width + posX] = pixelData;
                if (negX >= 0 && negX < m_width) buffer[negY * m_width + negX] = pixelData;
            }
        };

        long rx2 = rx * rx;
        long ry2 = ry * ry;
        long x = 0;
        long y = ry;
        long px = 0;
        long py = 2 * rx2 * y;

        // Região 1
        long p = (long)(ry2 - (rx2 * ry) + (0.25 * rx2));
        while (px < py) {
            plot4(x, y);
            x++;
            px += 2 * ry2;
            if (p < 0) p += ry2 + px;
            else {
                y--;
                py -= 2 * rx2;
                p += ry2 + px - py;
            }
        }

        // Região 2
        p = (long)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
        while (y >= 0) {
            plot4(x, y);
            y--;
            py -= 2 * rx2;
            if (p > 0) p += rx2 - py;
            else {
                x++;
                px += 2 * ry2;
                p += rx2 - py + px;
            }
        }
    }

    void Renderer2D::FillEllipse(int xc, int yc, int rx, int ry, short c, short col) {
        if (rx <= 0 || ry <= 0) return;

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* buffer = (uint32_t*)m_targetBuffer;

        // Lambda para desenhar uma linha horizontal sólida no buffer
        auto drawHLine = [&](int x1, int x2, int y) {
            if (y < 0 || y >= m_height) return;
            if (x1 > x2) std::swap(x1, x2);
            
            int left = (std::max)(0, x1);
            int right = (std::min)(m_width - 1, x2);
            
            if (left <= right) {
                uint32_t* p = &buffer[y * m_width + left];
                int count = right - left + 1;
                // Preenchimento linear de memória (Auge da performance)
                for (int i = 0; i < count; ++i) p[i] = pixelData;
            }
        };

        long rx2 = (long)rx * rx;
        long ry2 = (long)ry * ry;
        long x = 0;
        long y = ry;
        long px = 0;
        long py = 2 * rx2 * y;

        // Região 1
        long p = (long)(ry2 - (rx2 * ry) + (0.25 * rx2));
        while (px < py) {
            drawHLine(xc - x, xc + x, yc + y);
            drawHLine(xc - x, xc + x, yc - y);

            x++;
            px += 2 * ry2;
            if (p < 0) p += ry2 + px;
            else {
                y--;
                py -= 2 * rx2;
                p += ry2 + px - py;
            }
        }

        // Região 2
        p = (long)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
        while (y >= 0) {
            drawHLine(xc - x, xc + x, yc + y);
            drawHLine(xc - x, xc + x, yc - y);

            y--;
            py -= 2 * rx2;
            if (p > 0) p += rx2 - py;
            else {
                x++;
                px += 2 * ry2;
                p += rx2 - py + px;
            }
        }
    }

    void Renderer2D::Polygon(std::span<int*> points, short c, short col) {
        const size_t vertex = points.size();
        if (vertex < 3) return;

        // 1. Culling de Bounding Box
        // Acesso: points[i][0] é X, points[i][1] é Y
        int minX = points[0][0], maxX = points[0][0];
        int minY = points[0][1], maxY = points[0][1];
        
        for (int i = 1; i < vertex; ++i) {
            int x = points[i][0];
            int y = points[i][1];
            if (x < minX) minX = x; else if (x > maxX) maxX = x;
            if (y < minY) minY = y; else if (y > maxY) maxY = y;
        }

        if (maxX < 0 || minX >= m_width || maxY < 0 || minY >= m_height) return;

        // 2. Desenho das arestas
        for (int i = 0; i < vertex - 1; ++i) {
            Line(points[i][0], points[i][1], points[i+1][0], points[i+1][1], c, col);
        }

        // Fecha o polígono (último com o primeiro)
        Line(points[vertex-1][0], points[vertex-1][1], points[0][0], points[0][1], c, col);
    }

    void Renderer2D::FillPolygon(std::span<int*> points, short c, short col) {
        const size_t vertex = points.size();
        if (vertex < 3) return;

        // 1. Bounding Box e Setup de 32 bits
        int minY = points[0][1], maxY = points[0][1];
        for (size_t i = 1; i < vertex; ++i) {
            if (points[i][1] < minY) minY = points[i][1];
            if (points[i][1] > maxY) maxY = points[i][1];
        }

        minY = (std::max)(0, minY);
        maxY = (std::min)(m_height - 1, maxY);
        if (minY > maxY) return;

        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* buffer = (uint32_t*)m_targetBuffer;

        // Buffer fixo no stack (evita alocação no heap)
        // 128 nós suportam polígonos de até 128 arestas por scanline
        int nodes[128]; 

        // 2. Scanline Loop
        for (int y = minY; y <= maxY; ++y) {
            int nodeCount = 0;

            for (size_t i = 0; i < vertex; ++i) {
                size_t j = (i == vertex - 1) ? 0 : i + 1;
                
                int y0 = points[i][1], y1 = points[j][1];
                int x0 = points[i][0], x1 = points[j][0];

                if ((y0 < y && y1 >= y) || (y1 < y && y0 >= y)) {
                    // Cálculo de intersecção otimizado
                    // x = x0 + (y - y0) * (x1 - x0) / (y1 - y0)
                    nodes[nodeCount++] = x0 + (int)((float)(y - y0) * (x1 - x0) / (y1 - y0));
                    
                    if (nodeCount >= 128) break; // Segurança de stack
                }
            }

            // 3. Ordenação rápida (Insertion Sort é mais rápido que std::sort para N pequeno)
            for (int i = 1; i < nodeCount; ++i) {
                int j = i;
                while (j > 0 && nodes[j - 1] > nodes[j]) {
                    std::swap(nodes[j - 1], nodes[j]);
                    j--;
                }
            }

            // 4. Preenchimento de Memória Linear
            for (int i = 0; i < nodeCount; i += 2) {
                int left = (std::max)(0, nodes[i]);
                int right = (std::min)(m_width - 1, nodes[i + 1]);

                if (left <= right) {
                    uint32_t* pRow = &buffer[y * m_width + left];
                    // Loop de escrita direta (A CPU vetoriza isso automaticamente em 2026)
                    for (int x = 0; x <= (right - left); ++x) pRow[x] = pixelData;
                }
            }
        }
    }

    void Renderer2D::Char(int x, int y, wchar_t ch, int scale, short c, short col) {
        if (ch < 32 || ch > 90) ch = L'?';
        
        // ACESSO O(1): Direto no array, sem busca de hash
        int idx = static_cast<int>(ch) - 32;
        ScaledGlyph& glyph = m_glyphCache[idx];

        // Se a escala mudou ou o cache está vazio, "assa" o glifo
        if (glyph.data == nullptr || glyph.lastScale != scale) {
            if (glyph.data) delete[] glyph.data;
            
            glyph.w = 5 * scale;
            glyph.h = 5 * scale;
            glyph.lastScale = scale;
            glyph.data = new uint32_t[glyph.w * glyph.h](); 
            
            const uint8_t* raw = Font::Tiny5[idx];
            for (int gy = 0; gy < 5; ++gy) {
                if (raw[gy] == 0) continue;
                for (int gx = 0; gx < 5; ++gx) {
                    if (raw[gy] & (0x10 >> gx)) {
                        for (int i = 0; i < scale; ++i) {
                            uint32_t* rowStart = &glyph.data[((gy * scale) + i) * glyph.w + (gx * scale)];
                            std::fill_n(rowStart, scale, 1);
                        }
                    }
                }
            }
        }

        // --- RENDERIZAÇÃO ULTRA-RÁPIDA ---
        uint32_t pixelData = (uint32_t)col << 16 | (uint16_t)c;
        uint32_t* targetBuf = (uint32_t*)m_targetBuffer;

        int x0 = (std::max)(0, x), x1 = (std::min)(m_width, x + glyph.w);
        int y0 = (std::max)(0, y), y1 = (std::min)(m_height, y + glyph.h);
        if (x0 >= x1 || y0 >= y1) return;

        int widthPixels = x1 - x0;
        int srcStride = glyph.w;
        int destStride = m_width;

        // Aritmética de ponteiros pura para evitar recalcular offsets
        uint32_t* pDestRow = &targetBuf[y0 * destStride + x0];
        uint32_t* pSrcRow = &glyph.data[(y0 - y) * srcStride + (x0 - x)];

        for (int curY = y0; curY < y1; ++curY) {
            // Otimizado para vetorização automática (AVX/SSE)
            for (int i = 0; i < widthPixels; ++i) if (pSrcRow[i]) pDestRow[i] = pixelData;
            pDestRow += destStride;
            pSrcRow += srcStride;
        }
    }


    void Renderer2D::Text(int x, int y, const std::wstring& text, int scale, int spacing, short c, short col) {
        int cx = x;
        const int charSize = 5 * scale;
        const int advance = charSize + spacing;

        for (wchar_t ch : text) {
            if (ch == L'\n') {
                cx = x;
                y += advance;
                continue;
            }

            // Clipping de frustum: se o caractere está totalmente fora da tela, nem processa
            if (cx < m_width && cx + charSize > 0 && y < m_height && y + charSize > 0) {
                Char(cx, y, ch, scale, c, col);
            }
            
            cx += advance;
        }
    }

    Renderer2D::PPMAsset* Renderer2D::LoadPPMAsset(const std::wstring& filename) {
        FILE* f = _wfopen((std::wstring(L"" ASSETS_PATH) + filename).c_str(), L"rb");
        if (!f) return nullptr;

        char magic[3];
        int w, h, maxval;
        if (fscanf(f, "%2s %d %d %d", magic, &w, &h, &maxval) != 4 || strcmp(magic, "P6") != 0) {
            fclose(f);
            return nullptr;
        }
        fgetc(f); // Consumir o \n

        PPMAsset* asset = new PPMAsset();
        asset->w = w;
        asset->h = h;
        asset->data = new uint8_t[w * h * 3];
        
        fread(asset->data, 1, w * h * 3, f);
        fclose(f);
        return asset;
    }

    void Renderer2D::PutPPMAsset(PPMAsset* asset, int ox, int oy, int maxW, int maxH) {
        if (!asset || !asset->data || !m_targetBuffer) return;

        // 1. Dimensões alvo iniciais
        int dstW = (asset->w > maxW) ? maxW : asset->w;
        int dstH = (asset->h > maxH) ? maxH : asset->h;

        // 2. Clipping Geométrico Antecipado (Calcula os limites válidos uma única vez)
        int startY = (oy < 0) ? -oy : 0;
        int endY   = (oy + dstH > m_height) ? m_height - oy : dstH;
        int startX = (ox < 0) ? -ox : 0;
        int endX   = (ox + dstW > m_width) ? m_width - ox : dstW;

        if (startY >= endY || startX >= endX) return; // Fora da tela

        float stepX = (float)asset->w / (float)dstW;
        float stepY = (float)asset->h / (float)dstH;

        uint32_t* buffer = (uint32_t*)m_targetBuffer;
        int assetW3 = asset->w * 3;

        for (int y = startY; y < endY; y++) {
            // Offset de linha calculado apenas no loop externo
            uint32_t* pDestPixel = &buffer[(oy + y) * m_width + (ox + startX)];
            uint8_t* rowPtr = &asset->data[(int)(y * stepY) * assetW3];

            for (int x = startX; x < endX; x++) {
                // Amostragem direta via ponteiro
                uint8_t* p = &rowPtr[(int)(x * stepX) * 3];

                // Registradores locais para as cores
                uint8_t r = p[0], g = p[1], b = p[2];

                // Luminosidade (Média inteira rápida)
                int lum = (r + g + b) / 3;

                // Tabela de caracteres para eliminar múltiplos 'ifs' (Branchless)
                // Isso reduz a latência de pipeline da CPU drasticamente
                wchar_t ch = L' ';
                if (lum > 120) {
                    if (lum > 180) ch = PIXEL_SOLID; else ch = PIXEL_THREEQUARTERS;
                } else {
                    if (lum > 80) ch = PIXEL_HALF; else if (lum > 40) ch = PIXEL_QUARTER;
                }

                // Seleção de cor mantendo sua lógica original
                short col = FG_WHITE;
                if (r > g && r > b) col = FG_RED;
                else if (g > r && g > b) col = FG_GREEN;
                else if (b > r && b > g) col = FG_BLUE;
                else if (r > 200 && g > 200) col = FG_YELLOW;

                // Escrita direta com incremento de ponteiro
                *pDestPixel++ = (uint32_t)col << 16 | (uint16_t)ch;
            }
        }
    }

    /// ================= Renderer3D =================
    //
}
