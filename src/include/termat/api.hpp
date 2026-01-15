/// path: include/termat/api
//
#pragma once
#pragma comment(lib, "winmm.lib")
//
#ifndef UNICODE
#error Please enable UNICODE for your compiler!
#endif
//
// Resource imports
#include <windows.h>
//
#include <iostream>
#include <variant>
#include <chrono>
#include <vector>
#include <array>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
//
/// Public Imports
#include<termat/core.hpp>
#include<termat/debug.hpp>
#include<termat/gfx.hpp>   // em real uso
#include<termat/input.hpp>
#include<termat/net.hpp>
#include<termat/res.hpp>
#include<termat/scene.hpp>
#include<termat/sync.hpp>
#include<termat/sys.hpp>
#include<termat/ui.hpp>
//
/// NameSpace Raiz
namespace termat {
    //
    // Subsistemas
    namespace api   {} // Umbrella public API    (rotas de uso publico)
    namespace core  {} // Engine core            (serviços & eventos)
    namespace sys   {} // Low-level systems      ()
    namespace gfx   {} // Rendering manager      ()
    namespace input {} // Input manager          (teclado, mouse, ações)
    namespace scene {} // Scene manager          (Cenas, entidades e componentes)
    namespace res   {} // Resource manager       (arquivos, assets)
    namespace net   {} // Network manager        (cliente, servidor, protocolos)
    namespace sync  {} // Sync Manager           ()
    namespace ui    {} // User Interface Manager (widgets)
    namespace debug {} // Debug and tools        ()
    namespace util  {} // General utilities      ()
    //
    // Umbrella public API Namespace
    namespace api {
        //
        enum class ConsoleBackend {
            Legacy,
            WindowsTerminal
        };

        class Application {
        protected:
            //
            int m_width;  //
            int m_height; //

            //
            CHAR_INFO *m_bufScreen;  //
	        std::wstring m_sAppName; //

            // console handles
	        HANDLE m_hConsole;                                  // 
	        HANDLE m_hConsoleIn;                                // used:  may be different for legacy)
            HANDLE m_hOriginalConsole;                          // saved: 
            CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;   //
	        SMALL_RECT m_rectWindow;                            //

            // 
            bool m_isWindowsTerminal = false;       //
            bool m_usingCustomScreenBuffer = false; //

            //
            static inline std::atomic<bool> m_bAtomActive = false;  //
            static inline std::mutex m_muxGame;                     //
            static inline std::condition_variable m_cvGameFinished; //

            //
            friend int Run(Application& app); //

        public:
            // sub-sistemas
            termat::gfx::Renderer2D gfx2D;
            termat::gfx::Renderer3D gfx3D;

            Application(int width, int height, const std::wstring& appName = L"AppName", int fontw = 12, int fonth = 12) 
            : m_width(width), m_height(height), m_bufScreen(nullptr), // screen
              m_sAppName(appName), m_hOriginalConsole(INVALID_HANDLE_VALUE),
              m_hConsole(INVALID_HANDLE_VALUE), m_hConsoleIn(INVALID_HANDLE_VALUE)
            {
                // Save original handle (may be used to restore)
                m_hOriginalConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
                if (m_hOriginalConsole == INVALID_HANDLE_VALUE) Error(L"Bad STDOUT handle");
                if (m_hConsoleIn == INVALID_HANDLE_VALUE) Error(L"Bad STDIN handle");

                SetConsoleTitleW(m_sAppName.c_str()); // set app title 

                // Detect runtime environment
                m_isWindowsTerminal = IsWindowsTerminal();

                if (!m_isWindowsTerminal) {
                    // Legacy console path: create and use custom screen buffer, force sizes, set font, etc.
                    //
                    // Save original buffer handle (to restore on shutdown)
                    m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
                    if (m_hConsole == INVALID_HANDLE_VALUE) Error(L"CreateConsoleScreenBuffer");

                    // Change console visual size to a minimum so ScreenBuffer can shrink
                    // below the actual visual size
                    m_rectWindow = { 0, 0, 1, 1 };
                    SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

                    // Set the size of the screen buffer
                    COORD coord = { (short)m_width, (short)m_height };
                    if (!SetConsoleScreenBufferSize(m_hConsole, coord)) Error(L"SetConsoleScreenBufferSize");

                    // Assign screen buffer to the console
                    if (!SetConsoleActiveScreenBuffer(m_hConsole)) Error(L"SetConsoleActiveScreenBuffer");

                    // Set the font size now that the screen buffer has been assigned to the console
                    CONSOLE_FONT_INFOEX cfi;
                    cfi.cbSize = sizeof(cfi);
                    cfi.nFont = 0;
                    cfi.dwFontSize.X = fontw;
                    cfi.dwFontSize.Y = fonth;
                    cfi.FontFamily = FF_DONTCARE;
                    cfi.FontWeight = FW_NORMAL;
                    wcscpy_s(cfi.FaceName, L"Consolas");
                    if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)) Error(L"SetCurrentConsoleFontEx");

                    // Get screen buffer info and check the maximum allowed window size. Return
                    // error if exceeded, so user knows their dimensions/fontsize are too large
                    CONSOLE_SCREEN_BUFFER_INFO csbi;
                    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)) Error(L"GetConsoleScreenBufferInfo");
                    if (m_height > csbi.dwMaximumWindowSize.Y) Error(L"Screen Height / Font Height Too Big");
                    if (m_width > csbi.dwMaximumWindowSize.X) Error(L"Screen Width / Font Width Too Big");

                    // Set Physical Console Window Size
                    m_rectWindow = { 0, 0, static_cast<short>(m_width - 1), static_cast<short>(m_height - 1) };
                    if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow)) Error(L"SetConsoleWindowInfo");

                    // Set flags to allow mouse input		
                    if (!SetConsoleMode(m_hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) Error(L"SetConsoleMode");

                    // Allocate memory for screen buffer
                    allocateBuffer(m_width, m_height);
                    m_usingCustomScreenBuffer = true;

                    // m_bufScreen = new CHAR_INFO[m_width*m_height];
                    // memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_width * m_height);

                    // Hide cursor via Win32
                    CONSOLE_CURSOR_INFO cursorInfo;
                    GetConsoleCursorInfo(m_hConsole, &cursorInfo); // Obtém as configurações atuais
                    cursorInfo.bVisible = FALSE;                   // Define a visibilidade como falsa
                    SetConsoleCursorInfo(m_hConsole, &cursorInfo); // Aplica a mudança
                }
                else {
                    // Windows Terminal path: enable VT processing and adapt to current window size
                    EnableVTProcessing();

                    // Query current window size and adapt internal buffer
                    CONSOLE_SCREEN_BUFFER_INFO csbi;
                    if (!GetConsoleScreenBufferInfo(m_hOriginalConsole, &csbi)) {
                        // fallback to defaults passed by caller
                    } else {
                        // Use window size (columns = Right-Left+1 ; rows = Bottom-Top+1)
                        int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
                        // If passed width/height are larger, prefer the actual terminal current size
                        m_width = cols > 0 ? cols : m_width;
                        m_height = rows > 0 ? rows : m_height;
                    }

                    // allocate internal CHAR_INFO buffer (gfx draws here)
                    allocateBuffer(m_width, m_height);

                    // Hide cursor (both via Win32 and via VT)
                    CONSOLE_CURSOR_INFO ci{};
                    if (GetConsoleCursorInfo(m_hOriginalConsole, &ci)) {
                        ci.bVisible = FALSE;
                        SetConsoleCursorInfo(m_hOriginalConsole, &ci);
                    }
                    // also hide via VT (some terminals may still show)
                    sendRawAnsi("\x1b[?25l");

                    // Do not create/activate new screen buffer; we'll draw by converting to ANSI and writing bytes.
                    m_hConsole = m_hOriginalConsole;
                    m_usingCustomScreenBuffer = false;
                }

                //
                gfx2D.linkTarget(m_bufScreen, m_width, m_height);
                gfx3D.linkTarget(m_bufScreen, m_width, m_height);

                SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
            };
            ~Application() {
                // restore original console buffer if we replaced it
                if (m_usingCustomScreenBuffer && m_hOriginalConsole != INVALID_HANDLE_VALUE) SetConsoleActiveScreenBuffer(m_hOriginalConsole);

                // If VT: restore cursor visibility and disable VT? We leave modes as-is.
                if (m_bufScreen) { delete[] m_bufScreen; m_bufScreen = nullptr; }
            };

            // Instancias de Acesso
            std::array<int, 2> getWindowSize() const { return { m_width - 1, m_height - 1 }; }

            // Instancias de Definição
            void setTitle(const std::wstring& title) {
                m_sAppName = title;
                SetConsoleTitleW(m_sAppName.c_str());
            }

            void displayBuffer() {
                // Legacy: use WriteConsoleOutputW directly
                if (!m_isWindowsTerminal) WriteConsoleOutputW(m_hConsole, m_bufScreen, {(short)m_width, (short)m_height}, {0,0}, &m_rectWindow); 
                else {
                    // For WT: check if terminal was resized. If yes, adapt internal buffer
                    refreshSizeIfNeeded();
                    displayBufferVT();
                }
            }

            // Instancias de Execução
            virtual void onInit() {}
            virtual void onUpdate(double dt) {}
            virtual void onRender() {}
            virtual void onEvent() {}
            virtual void onShutdown() {}

        protected:
            // -- helpers --
            
            bool IsWindowsTerminal() {
                // check WT_SESSION env var (common method)
                char* wt_session = nullptr;
                size_t len = 0;
                _dupenv_s(&wt_session, &len, "WT_SESSION");
                bool isWT = (wt_session != nullptr && len > 0);
                free(wt_session);
                return isWT;
            }

            void EnableVTProcessing() {
                DWORD mode = 0;
                HANDLE hOut = m_hOriginalConsole;
                if (!GetConsoleMode(hOut, &mode)) {
                    // ignore
                    return;
                }
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
                SetConsoleMode(hOut, mode);
            }

            void allocateBuffer(int w, int h) {
                if (m_bufScreen) delete[] m_bufScreen;
                m_bufScreen = new CHAR_INFO[w * h];
                // zero init
                memset(m_bufScreen, 0, sizeof(CHAR_INFO) * w * h);
                m_width = w;
                m_height = h;
            }

            void refreshSizeIfNeeded() {
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                if (!GetConsoleScreenBufferInfo(m_hOriginalConsole, &csbi)) return;
                int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
                if (cols != m_width || rows != m_height) {
                    // reallocate buffer and relink renderers
                    allocateBuffer(cols, rows);
                    gfx2D.linkTarget(m_bufScreen, m_width, m_height);
                    gfx3D.linkTarget(m_bufScreen, m_width, m_height);
                }
            }

            // Convert WCHAR to UTF-8 (single char)
            static std::string wchar_to_utf8(wchar_t wc) {
                if (wc == 0) return std::string(1, ' ');
                wchar_t src[2] = { wc, 0 };
                int needed = WideCharToMultiByte(CP_UTF8, 0, src, 1, NULL, 0, NULL, NULL);
                if (needed <= 0) return std::string(1, ' ');
                std::string out;
                out.resize(needed);
                WideCharToMultiByte(CP_UTF8, 0, src, 1, &out[0], needed, NULL, NULL);
                return out;
            }

            // Map Windows console attribute to ANSI SGR sequences (fg/bg)
            static std::string attr_to_sgr(WORD attr) {
                // Foreground
                int fg = (attr & FOREGROUND_RED ? 1 : 0) |
                        (attr & FOREGROUND_GREEN ? 2 : 0) |
                        (attr & FOREGROUND_BLUE ? 4 : 0);
                bool fg_intense = (attr & FOREGROUND_INTENSITY) != 0;
                int fg_code = (fg_intense ? 90 : 30) + fg; // 30-37 or 90-97

                // Background
                int bg = (attr & BACKGROUND_RED ? 1 : 0) |
                        (attr & BACKGROUND_GREEN ? 2 : 0) |
                        (attr & BACKGROUND_BLUE ? 4 : 0);
                bool bg_intense = (attr & BACKGROUND_INTENSITY) != 0;
                int bg_code = (bg_intense ? 100 : 40) + bg; // 40-47 or 100-107

                // If no color (both zero) still return reset? We'll return codes
                char buf[64];
                int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dm", fg_code, bg_code);
                return std::string(buf, len);
            }

            // Send raw ANSI to terminal (used for control sequences)
            void sendRawAnsi(const char* s) {
                DWORD written = 0;
                WriteFile(m_hOriginalConsole, s, (DWORD)strlen(s), &written, NULL);
            }

            // Build entire frame as UTF-8 with ANSI escapes; write in single WriteFile call.
            void displayBufferVT() {
                if (!m_bufScreen) return;
                // We'll iterate rows, build a large std::string (reserve approximate size)
                std::string out;
                out.reserve((size_t)m_width * (size_t)m_height * 4 + 1024);

                // Move cursor to home
                out += "\x1b[H";

                WORD prevAttr = 0xFFFF; // impossible initial value to force SGR emit
                for (int y = 0; y < m_height; ++y) {
                    for (int x = 0; x < m_width; ++x) {
                        const CHAR_INFO &ci = m_bufScreen[y * m_width + x];
                        WORD attr = ci.Attributes;
                        if (attr != prevAttr) {
                            out += attr_to_sgr(attr);
                            prevAttr = attr;
                        }
                        wchar_t wc = ci.Char.UnicodeChar;
                        // convert char to utf8 and append
                        std::string utf8 = wchar_to_utf8(wc);
                        out += utf8;
                    }
                    // At end of line: clear remainder? but we assume buffer covers whole width.
                    if (y < m_height - 1) out += '\n';
                }
                // Reset attributes at the end (so cursor/terminal returns to normal)
                out += "\x1b[0m";

                // Write all bytes at once
                DWORD written = 0;
                WriteFile(m_hOriginalConsole, out.c_str(), (DWORD)out.size(), &written, NULL);
            }

            //
            int Error(const wchar_t *msg) {
	        	wchar_t buf[256];
		        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		        if (m_hOriginalConsole != INVALID_HANDLE_VALUE) SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		        wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
	        	return 0;
        	}

            //
            static BOOL WINAPI CloseHandler(DWORD evt) {
                // Note this gets called in a seperate OS thread, so it must
                // only exit when the game has finished cleaning up, or else
                // the process will be killed before OnUserDestroy() has finished
                if (evt == CTRL_CLOSE_EVENT) {
                    m_bAtomActive = false;

                    // Wait for thread to be exited
                    std::unique_lock<std::mutex> ul(m_muxGame);
                    m_cvGameFinished.wait(ul);
                }
                return TRUE;
            }
        };

        //
        inline int Run(Application& app) {
            //
            app.m_bAtomActive = true;
            app.onInit();

            //
            while(app.m_bAtomActive) {
                app.onUpdate(1);
                app.onRender();
                app.onEvent();
                app.displayBuffer();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // 
            app.onShutdown();
            std::lock_guard<std::mutex> lg(app.m_muxGame);
            app.m_cvGameFinished.notify_one();

            return 0; 
        };
    }

    // rotas de recursos dos sub-sistemas
    using PPMAsset = gfx::Renderer2D::PPMAsset;
}
//