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

public:
	HelloApp() : Application(280, 180, L"AppName", 3, 3) {}

    void onInit() override {
        gfx2D.String(0, 0, L"[HelloApp][Msg]: onInit()");
    }

    void onUpdate(double dt) override {
        // exemplo: Atualização te tempo
        acc += dt;
    }

    void onRender() override {
        // GFX 2D
        const std::wstring str = std::format(L"HELLO WORLD! - FRAME:{}", acc);
        gfx2D.String(0, 1, str);
        gfx2D.Line(0, 5, 10, 10);
        gfx2D.Triangle(0, 15, 0, 20, 10, 20);
        gfx2D.FillTriangle(0, 25, 0, 30, 10, 30);
        gfx2D.Circle(5, 40, 5);
        gfx2D.FillCircle(5, 55, 5);
        gfx2D.Rect(15, 5, 25, 15);
        gfx2D.FillRect(15, 20, 25, 30);
        gfx2D.Text(15, 35, L"HELLO");
    }

    void onShutdown() override {
        gfx2D.String(0, 0, L"[HelloApp][Msg]: onShutdown()");
    }
};
//
int main() {
    HelloApp app;
    // Chama a função pública de entrada — implementada no core (termat lib)
    // Certifique-se de linkar com a lib "termat" (termat.lib) ao compilar.
    return termat::api::Run(app);
}