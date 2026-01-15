// path: src/main.cpp
//
#include<iostream>
#include<format>
//
#include<termat/api.hpp>
#include<termat/samples.hpp>
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
// Chama a função pública de entrada — implementada no core (termat lib)
// Certifique-se de linkar com a lib "termat" (termat.lib) ao compilar.
int main() {
    IntroApp  app;
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