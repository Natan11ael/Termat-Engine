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
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
//
#include<termat/gfx.hpp>

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
        class Application {
        protected:
            // Instancias da Aplicação
            int m_width;
            int m_height;
            CHAR_INFO *m_bufScreen;
	        std::wstring m_sAppName;
	        HANDLE m_hOriginalConsole;
	        CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;
	        HANDLE m_hConsole;
	        HANDLE m_hConsoleIn;
	        SMALL_RECT m_rectWindow;

            static inline std::atomic<bool> m_bAtomActive = false;
            static inline std::mutex m_muxGame;
            static inline std::condition_variable m_cvGameFinished;

            friend int Run(Application& app); 

        public:
            // Instâncias dos sub-sistemas
            termat::gfx::Renderer2D gfx2D;
            termat::gfx::Renderer3D gfx3D;

            Application(int width, int height, const std::wstring& appName = L"AppName", int fontw = 12, int fonth = 12) {
                // 1. Set
                m_width = width;
                m_height = height;

                // 2. Get
                m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		        m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

                // 3. Set App Name
                m_sAppName = appName;
                SetConsoleTitleW(m_sAppName.c_str());

                // 4.
                if (m_hConsole == INVALID_HANDLE_VALUE) Error(L"Bad Handle");

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
		        m_bufScreen = new CHAR_INFO[m_width*m_height];
		        memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_width * m_height);

		        SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);

                //
                CONSOLE_CURSOR_INFO cursorInfo;
                GetConsoleCursorInfo(m_hConsole, &cursorInfo); // Obtém as configurações atuais
                cursorInfo.bVisible = FALSE;                   // Define a visibilidade como falsa
                SetConsoleCursorInfo(m_hConsole, &cursorInfo); // Aplica a mudança

                gfx2D.linkTarget(m_bufScreen, m_width, m_height);
                gfx3D.linkTarget(m_bufScreen, m_width, m_height);
            };
            ~Application() {
                SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		        delete[] m_bufScreen;
            };

            // Instancias de Definição
            void setTitle(const std::wstring& title) {
                m_sAppName = title;
                SetConsoleTitleW(m_sAppName.c_str());
            }

            void displayBuffer() {
                WriteConsoleOutputW(m_hConsole, m_bufScreen, {(short)m_width, (short)m_height}, {0,0}, &m_rectWindow);
            }

            // Instancias de Execução
            virtual void onInit() {}
            virtual void onUpdate(double dt) {}
            virtual void onRender() {}
            virtual void onEvent() {}
            virtual void onShutdown() {}

        protected:
            int Error(const wchar_t *msg) {
	        	wchar_t buf[256];
		        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		        SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		        wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
	        	return 0;
        	}

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
                return true;
            }
        };

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
}
//
/// Public Imports
#include<termat/core.hpp>
#include<termat/debug.hpp>
//#include<termat/gfx.hpp>
#include<termat/input.hpp>
#include<termat/net.hpp>
#include<termat/res.hpp>
#include<termat/scene.hpp>
#include<termat/sync.hpp>
#include<termat/sys.hpp>
#include<termat/ui.hpp>