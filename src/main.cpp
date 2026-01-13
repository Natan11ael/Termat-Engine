// path: src/main.cpp
//
#include<iostream>
#include<format>
//
#include<termat/api.hpp>
//
// API Template: Hello World
class HelloApp : public termat::api::Application {
private:
    double acc = 0.0;
    termat::PPMAsset * img = nullptr;

public:
	HelloApp() : Application(128, 64, L"AppName", 8, 8) {}

    void onInit() override {
        gfx2D.String(0, 0, L"[HelloApp][Msg]: onInit()");
        img = gfx2D.LoadPPMAsset(L"Sprite-32x32.ppm");
    }

    void onUpdate(double dt) override {
        // exemplo: Atualização te tempo
        acc += dt;
    }

    void onRender() override {
        // GFX 2D
        auto [ w, h ] = getWindowSize();
        gfx2D.FillRect(0, 0, w, h, termat::gfx::PIXEL_SOLID, termat::gfx::FG_BLACK | termat::gfx::BG_BLACK);
        
        const int x = 5;
        auto start = std::chrono::high_resolution_clock::now();

        // null // ~.6 us
        gfx2D.Pixel(x*0, 1); // ~1 us
        gfx2D.Line(x*1, 1, x*2-1, x); // ~3 us
        gfx2D.Rect(x*2+1, 1, x*3+1, x+1); // ~3 us
        gfx2D.FillRect(x*3+2, 1, x*4+2, x+1); // ~1 us
        gfx2D.Triangle(x*4+3, 1, x*4+3, x, x*5+2, 1); // ~4 us
        gfx2D.FillTriangle(x*5+4, 1, x*5+4, x, x*6+3, 1); // ~4 us
        gfx2D.Ellipse(x*6+7, 3, 2, 2); // ~4 us
        gfx2D.FillEllipse(x*7+8, 3, 2, 2); // ~4 us

        int ap1[] = {x*8+7, 1};
        int ap2[] = {x*8+7, x};
        int ap3[] = {x*9+4, x-3};
        int ap4[] = {x*9+7, 1};
        int* apoints[] = {ap1, ap2, ap3, ap4};
        gfx2D.Polygon(apoints); // ~9 us

        int bp1[] = {x*9+9, 0};
        int bp2[] = {x*9+9, x};
        int bp3[] = {x*10+6, x-3};
        int bp4[] = {x*10+10, 0};
        int* bpoints[] = {bp1, bp2, bp3, bp4};
        gfx2D.FillPolygon(bpoints); // ~10 us

        gfx2D.Text(x*11+5, 1, L"HI"); // ~60 us - *Nrnhum progresso
        gfx2D.PutPPMAsset(img, 0, 5); // ~100 us
        
        // all // ~202 us

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration = end - start;

        const std::wstring str = std::format(L"HELLO WORLD! FRAME:{} DRAWNER:{}", acc, duration);
        gfx2D.String(0, 0, str);
    }

    void onShutdown() override {
        gfx2D.String(0, 0, L"[HelloApp][Msg]: onShutdown()");
    }
};
//
// API Template: Animação e Movimento
// Este template demonstra como usar o tempo acumulado para criar movimentos senoidais,
// renderização de sprites (PPM) com bounce e efeitos de partículas simples.
class AnimationApp : public termat::api::Application {
private:
    double acc = 0.0;
    termat::PPMAsset* sprite = nullptr;
    
    struct Particle {
        float x, y, vx, vy;
        wchar_t symbol;
    };
    std::vector<Particle> particles;

public:
    // Configuração para alta resolução de caracteres (4x4) resultando em 256x128
    AnimationApp() : Application(256, 128, L"Termat Engine - Animation Demo", 4, 4) {}

    void onInit() override {
        // Carrega o asset uma única vez no início
        sprite = gfx2D.LoadPPMAsset(L"Sprite-32x32.ppm");

        // Inicializa algumas "partículas" de fundo
        for(int i = 0; i < 50; i++) {
            particles.push_back({
                (float)(rand() % 256), (float)(rand() % 128),
                0.0f, (float)(rand() % 10 + 5) / 10.0f, // Velocidade de queda
                L'.'
            });
        }
    }

    void onUpdate(double dt) override {
        acc += dt;

        // Atualiza partículas (Efeito de neve/chuva)
        for(auto& p : particles) {
            p.y += p.vy;
            if (p.y > 128) {
                p.y = -1;
                p.x = rand() % 256;
            }
        }
    }

    void onRender() override {
        auto [w, h] = getWindowSize();
        
        // 1. Limpar tela com fade azul escuro (BG_BLUE para profundidade)
        gfx2D.FillRect(0, 0, w, h, termat::gfx::PIXEL_SOLID, termat::gfx::FG_BLACK | termat::gfx::BG_BLACK);

        // 2. Desenhar partículas de fundo
        for(const auto& p : particles) {
            gfx2D.Pixel((int)p.x, (int)p.y, p.symbol, termat::gfx::FG_GREY);
        }

        // 3. Animação de Círculos Oscilantes (Trigonometria)
        for (int i = 0; i < 8; i++) {
            float offset = i * (6.28f / 8.0f);
            int x = (w / 2) + (int)(std::cos(acc + offset) * 60.0f);
            int y = (h / 2) + (int)(std::sin(acc * 1.5f + offset) * 30.0f);
            
            short col = (i % 2 == 0) ? termat::gfx::FG_CYAN : termat::gfx::FG_MAGENTA;
            gfx2D.FillEllipse(x, y, 4, 4, termat::gfx::PIXEL_HALF, col);
        }

        // 4. Renderizar Sprite com "Bounce" (Efeito de pulo)
        if (sprite) {
            int bounceY = (h / 2 - 16) + (int)(std::abs(std::sin(acc * 3.0)) * -20.0);
            int centerX = (w / 2 - 16);
            
            // Desenha sombra do sprite
            gfx2D.FillEllipse(w/2, h/2 + 20, 15 + (bounceY/10), 5, termat::gfx::PIXEL_QUARTER, termat::gfx::FG_GREY);
            
            // Desenha o asset PPM otimizado
            gfx2D.PutPPMAsset(sprite, centerX, bounceY, 32, 32);
        }

        // Moldura dinâmica
        gfx2D.Rect(2, 2, w-3, h-3, termat::gfx::PIXEL_SOLID, (short)(acc * 10) % 15 + 1);

        // 5. UI de Telemetria
        gfx2D.FillRect(0, 0, 45, 10, termat::gfx::PIXEL_SOLID, termat::gfx::FG_BLACK | termat::gfx::BG_WHITE);
        gfx2D.String(1, 1, L"TIME: " + std::to_wstring(acc).substr(0, 5), termat::gfx::FG_BLACK);
        gfx2D.String(1, 4, L"MEM OTIMIZADA", termat::gfx::FG_RED);
    }

    void onShutdown() override {
        // O PPMAsset é limpo pelo destrutor automático se configurado, 
        // ou manualmente aqui se preferir.
    }
};
//
int main() {
    AnimationApp app;
    // Chama a função pública de entrada — implementada no core (termat lib)
    // Certifique-se de linkar com a lib "termat" (termat.lib) ao compilar.
    return termat::api::Run(app);
}
// 8:8
// - 64x64 - 1:1
// - 128x64 - 2:1
//
// 6:6
// - 96x96 - 1:1
// - 128x96 - 2:1
//
// 4:4
// - 128x128 - 1:1
// - 256x128 - 2:1
//
// 2:2
// - 256x256 - 1:1
// - 512x256 - 2:1