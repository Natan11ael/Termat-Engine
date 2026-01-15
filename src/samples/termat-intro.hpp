// path: 
//
#include<termat/api.hpp>
#include<algorithm>
#include<string>
#include<format>
//
// Termat text animation example
// - Digita "Termat Engine" lentamente
// - Depois faz uma transição que reduz o texto para "T:_" (com cursor piscando)
// - Todos os elementos têm FG e BG explicitamente desenhados
#include <termat/api.hpp>
#include <algorithm>
#include <string>
#include <format>

class IntroApp : public termat::api::Application {
private:
    double acc = 0.0;

    double charTimer = 0.0;
    double phaseTimer = 0.0;
    double cursorTimer = 0.0;

    int visibleChars = 0;

    bool typing = true;
    bool holding = false;
    bool erasing = false;
    bool addingTarget = false;
    bool blinkingEnd = false;
    bool fadingOut = false;

    bool cursorOn = true;
    int blinkCount = 0;        // quantidade de piscadas do _
    double fadeAlpha = 0.0;    // 0 = visível, 1 = invisível

    const std::wstring full   = L"Termat Engine";
    std::wstring display      = L"";

    const int CHAR_W = 6;
    const int CHAR_H = 7;

public:
    IntroApp() : Application(128, 64, L"Termat Intro Animation", 8, 8) {}

    void onInit() override {
        display.clear();
    }

    void onUpdate(double dt) override {
        acc += dt;
        charTimer += dt;
        phaseTimer += dt;
        cursorTimer += dt;

        // cursor base
        if (cursorTimer >= 16) {
            cursorTimer = 0.0;
            cursorOn = !cursorOn;
        }

        // ---- DIGITAÇÃO ----
        if (typing) {
            if (charTimer >= 0.12) {
                charTimer = 0.0;
                if (visibleChars < (int)full.size()) {
                    visibleChars++;
                    display = full.substr(0, visibleChars);
                } else {
                    typing = false;
                    holding = true;
                    phaseTimer = 0.0;
                }
            }
            return;
        }

        // ---- PAUSA ----
        if (holding) {
            if (phaseTimer >= 16) {
                holding = false;
                erasing = true;
                charTimer = 0.0;
            }
            return;
        }

        // ---- APAGAR ----
        if (erasing) {
            if (charTimer >= 0.07) {
                charTimer = 0.0;
                if (visibleChars > 1) {
                    visibleChars--;
                    display = full.substr(0, visibleChars);
                } else {
                    erasing = false;
                    addingTarget = true;
                    charTimer = 0.0;
                }
            }
            return;
        }

        // ---- ADICIONAR ":_" ----
        if (addingTarget) {
            if (charTimer >= 0.15) {
                charTimer = 0.0;
                display = L"T:_";
                addingTarget = false;
                blinkingEnd = true;
                blinkCount = 0;
            }
            return;
        }

        // ---- PISCAR "_" 3 VEZES ----
        if (blinkingEnd) {
            if (cursorTimer >= 8) {
                cursorTimer = 0.0;
                cursorOn = !cursorOn;

                if (!cursorOn) {
                    blinkCount++;
                    if (blinkCount >= 3) {
                        blinkingEnd = false;
                        fadingOut = true;
                        fadeAlpha = 0.0;
                    }
                }
            }
            return;
        }

        // ---- FADE OUT ----
        if (fadingOut) {
            fadeAlpha += dt * 0.06;
            if (fadeAlpha >= 1.0) {
                fadeAlpha = 1.0;
            }
        }
    }

    void onRender() override {
        auto [w, h] = getWindowSize();

        gfx2D.FillRect(
            0, 0, w, h,
            termat::gfx::PIXEL_SOLID,
            termat::gfx::FG_BLACK | termat::gfx::BG_BLACK
        );

        if (fadeAlpha >= 1.0) return;

        int textWidth = (int)display.size() * CHAR_W;
        int x = (w / 2) - (textWidth / 2);
        int y = (h / 2) - (CHAR_H / 2);

        std::wstring out = display;

        if (display == L"T:_") {
            if (!cursorOn) {
                out = L"T: ";
            }
        }

        short fg = termat::gfx::FG_WHITE;
        if (fadeAlpha > 0.0) {
            if (fadeAlpha > 0.66) fg = termat::gfx::FG_DARK_GREY;
            else if (fadeAlpha > 0.33) fg = termat::gfx::FG_GREY;
        }

        const int pad = 2;
        gfx2D.Rect(x - pad, y - pad, x + textWidth + pad, y + CHAR_H, termat::gfx::PIXEL_SOLID, fg);
        gfx2D.Text(x, y, out, 1, 1, termat::gfx::PIXEL_SOLID, fg);
    }

    void onShutdown() override {}
};