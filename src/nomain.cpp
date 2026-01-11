// Termat : main.cpp : Função principal do programa.
//
/// - Visual Studios:
// Executar programa: Ctrl + F5 > Iniciar Sem Depuração
// Depurar programa:  F5 		> Iniciar Depuração
//
/// - Visual Studios:
// Depurar programa: F5  		> Iniciar Depuração
//
#include <cmath>
#include <iomanip>
#include <iostream>
//
#include <array>
#include <string>
#include <string_view>
#include <vector>
//
#include <chrono>
#include <format>
#include <functional>
#include <thread>
//
#include <windows.h>
#ifdef _WIN32
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#endif

//
//
WORD getColor(int fg, int bg)
{
	return ((bg & 0x0F) << 4) | (fg & 0x0F); //
}

class Inputs
{
private:
	HANDLE m_hInput;
	// Usamos short para alinhar com o retorno do Windows, mas bool funciona
	// também. Otimização de cache: keys e mouse_pos próximos na memória.
	std::array<bool, 256> m_keys{false};
	std::pair<int, int> m_mouse_pos{0, 0};

	// Buffer estático para evitar alocação dinâmica no heap a cada frame.
	// 128 eventos por frame é mais do que suficiente para humanos.
	static const int BUFFER_SIZE = 128;
	INPUT_RECORD m_eventBuffer[BUFFER_SIZE];

	long long m_lastProcessTimeMicros = 0;

public:
	Inputs() : m_hInput(GetStdHandle(STD_INPUT_HANDLE))
	{
		// Removemos ENABLE_WINDOW_INPUT se não for usar redimensionamento para
		// evitar lixo no buffer
		SetConsoleMode(m_hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
	}

	void
	Update()
	{
		auto start = std::chrono::high_resolution_clock::now();

		DWORD numEvents = 0;

		// Verifica quantos eventos estão pendentes
		GetNumberOfConsoleInputEvents(m_hInput, &numEvents);

		if (numEvents > 0)
		{
			DWORD eventsRead = 0;

			// OTIMIZAÇÃO CRÍTICA:
			// 1. Lê em blocos (batch processing).
			// 2. Usa buffer estático (sem new/malloc).
			// 3. Lê tanto Teclado quanto Mouse no mesmo loop (elimina
			// GetAsyncKeyState).

			// Loop para drenar tudo se houver mais que o tamanho do buffer (raro)
			while (numEvents > 0)
			{
				DWORD toRead = (numEvents > BUFFER_SIZE) ? BUFFER_SIZE : numEvents;

				if (ReadConsoleInputW(m_hInput, m_eventBuffer, toRead,
									  &eventsRead))
				{
					for (DWORD i = 0; i < eventsRead; i++)
					{
						switch (m_eventBuffer[i].EventType)
						{
						case KEY_EVENT:
						{
							// Atualiza o estado da tecla baseado no evento
							// "Down" ou "Up"
							const auto &ke = m_eventBuffer[i].Event.KeyEvent;
							// Previne leitura fora do array (segurança)
							if (ke.wVirtualKeyCode < 256)
							{
								m_keys[ke.wVirtualKeyCode] = ke.bKeyDown;
							}
							break;
						}

						case MOUSE_EVENT:
						{
							const auto &me = m_eventBuffer[i].Event.MouseEvent;
							// O evento MOUSE_MOVED é disparado sempre que o
							// mouse mexe
							m_mouse_pos = {me.dwMousePosition.X / 2,
										   me.dwMousePosition.Y};
							break;
						}

						// Ignoramos FOCUS_EVENT, MENU_EVENT, etc. por
						// performance
						default:
							break;
						}
					}
				}
				// Decrementa o que já processamos
				numEvents -= eventsRead;
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		m_lastProcessTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
									  .count();
	}

	// Getters - Inline para velocidade máxima
	// inline é implícito dentro da classe, mas reforça a intenção
	inline bool
	isPress(int vKey) const
	{
		// Proteção simples de bounds
		if (vKey >= 0 && vKey < 256)
			return m_keys[vKey];
		return false;
	}

	inline std::pair<int, int>
	mouse_pos() const
	{
		return m_mouse_pos;
	}

	long long
	getProcessTimeMicros() const
	{
		return m_lastProcessTimeMicros;
	}
};

// Gerenciador de tempo
class Time
{
private:
	using clock = std::chrono::high_resolution_clock;
	using duration_d = std::chrono::duration<double>;

	std::chrono::time_point<clock> m_startTime;
	std::chrono::time_point<clock> m_lastFrameTime;

	double m_targetFrameTime; // Tempo alvo em segundos (ex: 0.01666)
	float m_deltaTime;
	float m_totalTime;
	unsigned int m_frameCount;

public:
	Time(int targetFPS)
		: m_deltaTime(0.0f), m_totalTime(0.0f), m_frameCount(0)
	{
		// Define o target preciso (1.0 / 60.0 = 0.016666...)
		if (targetFPS > 0)
			m_targetFrameTime = 1.0 / static_cast<double>(targetFPS);
		else
			m_targetFrameTime = 0.0;

// OTIMIZAÇÃO WINDOWS: Aumenta a granularidade do timer para 1ms
// Sem isso, o sleep_for pode errar por até 15ms.
#ifdef _WIN32
		timeBeginPeriod(1);
#endif

		m_startTime = clock::now();
		m_lastFrameTime = m_startTime;
	}

	~Time()
	{
#ifdef _WIN32
		timeEndPeriod(1);
#endif
	}

	// Chama isso bem no início do loop
	void
	Update()
	{
		auto currentFrameTime = clock::now();

		// Calcula DeltaTime
		std::chrono::duration<float> diff = currentFrameTime - m_lastFrameTime;
		m_deltaTime = diff.count();

		// Evita deltas gigantes se o jogo travar (Clamp opcional, mas recomendado)
		if (m_deltaTime > 0.1f)
			m_deltaTime = 0.1f;

		m_lastFrameTime = currentFrameTime;
		m_totalTime += m_deltaTime;
		m_frameCount++;
	}

	// Chama isso bem no final do loop, após renderizar
	void
	LimitFPS()
	{
		if (m_targetFrameTime <= 0.0)
			return;

		// Quanto tempo passou desde o início deste frame (Update)?
		auto now = clock::now();
		duration_d processingTime = now - m_lastFrameTime;

		// Quanto tempo precisamos esperar?
		double waitSeconds = m_targetFrameTime - processingTime.count();

		// Se estamos adiantados, precisamos esperar
		if (waitSeconds < 0)
			return;

		// TÉCNICA HÍBRIDA (Sleep + SpinLock)

		// 1. Sleep: Usamos sleep para a maior parte do tempo para economizar CPU.
		// Mas deixamos uma margem de segurança de ~2ms porque o sleep é impreciso.
		if (waitSeconds > 0.002) // Se falta mais de 2ms
		{
			std::this_thread::sleep_for(
				std::chrono::duration<double>(waitSeconds - 0.002));
		}

		// 2. SpinLock (Busy Wait): Nos últimos 1-2ms, fazemos um loop vazio.
		// Isso queima um pouquinho de CPU, mas garante precisão de nanossegundos.
		do
		{
			now = clock::now();
			processingTime = now - m_lastFrameTime;
		} while (processingTime.count() < m_targetFrameTime);
	}

	// Getters
	float
	DeltaTime() const
	{
		return m_deltaTime;
	}
	float
	TotalTime() const
	{
		return m_totalTime;
	}
	unsigned int
	FrameCount() const
	{
		return m_frameCount;
	}

	// FPS Médio suavizado é melhor para display, mas o instantâneo é:
	float
	FPS() const
	{
		return m_deltaTime > 0 ? 1.0f / m_deltaTime : 0.0f;
	}
};

// Classe para manipulação do console como uma tela gráfica
class Termat {
private:
	std::vector<CHAR_INFO> buffer;
	COORD bufferSize;
	SMALL_RECT writeRegion;
	HANDLE hConsole;
	const int width;
	const int height;
	Time time = Time(60);

public:
	Inputs inputs = Inputs();

	Termat(SHORT w, SHORT h, char asciiChar = ' ', WORD attr = getColor(15, 0))
		: width(w), height(h), hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
		  bufferSize{SHORT(w * 2), h},
		  writeRegion{0, 0, SHORT(w * 2 - 1), SHORT(h - 1)},
		  buffer(w * 2 * h) 
	{
		// 1. Configuração Física da Janela
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

		// 2. Configuração do Modo do Console
		DWORD dwMode = 0;
		GetConsoleMode(hConsole, &dwMode);
		SetConsoleMode(hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

		// 3. Ajuste de Buffer e Tela
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		COORD newSize = {(SHORT)(csbi.srWindow.Right - csbi.srWindow.Left + 1), (SHORT)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)};
		SetConsoleScreenBufferSize(hConsole, newSize);

		this->setBuffer(asciiChar, attr); // Limpa o buffer com o caracter desejado
	}

	// Insere um Char dentro do buffer
	void setCh(int x, int y, WCHAR ch, WORD attributes) {
		if (x <= 0 && y <= 0 && x > width && y > height) return;

		int idx = y * (width * 2) + x * 2;
		CHAR_INFO ci{};
		ci.Char.UnicodeChar = ch;
		ci.Attributes = attributes;
		buffer[idx] = ci;
		buffer[idx + 1] = ci;
	}

	// Insere um String dentro do buffer
	template <typename... Args>
	void setString(int x, int y, WORD attributes, std::string_view fmt, Args &&...args) {
		try {
			std::string text = std::vformat(fmt, std::make_format_args(args...));

			if (y < 0 || y >= height) return;

			for (size_t i = 0; i < text.size(); i++) {
				int px = x + (int)i;
				if (px < 0 || px >= width * 2) break;

				int idx = y * (width * 2) + px;
				buffer[idx].Char.UnicodeChar = (WCHAR)text[i];
				buffer[idx].Attributes = attributes;
			}
		}
		catch (const std::exception &) {}
	}

	// Preenche o buffer
	void setBuffer(char asciiChar = ' ', WORD attributes = getColor(15, 0)) {
		static const CHAR_INFO blank = {L' ', getColor(15, 0)};
		std::fill(buffer.begin(), buffer.end(), blank);
	}

	// Executa o loop principal
	void run(std::function<void(Time, const Inputs &)> logic) {
		// OTIMIZAÇÃO: Cache de handles e configurações
		const HANDLE hOut = hConsole;

		while (true) {
			// Update Time
			time.Update();
			inputs.Update();
			if (inputs.isPress(VK_ESCAPE)) break;

			//
			setBuffer();
			logic(time, inputs);
			setString(0, height - 1, getColor(15, 0), "Input Latency: {} us", inputs.getProcessTimeMicros());

			// Display & Sync
			WriteConsoleOutputW(hOut, buffer.data(), bufferSize, {0, 0}, &writeRegion); // Display
			time.LimitFPS(); // Sync
		}
		setBuffer();
		WriteConsoleOutputW(hConsole, buffer.data(), bufferSize, COORD{0, 0}, &writeRegion);
	}
};
//
int main() {
	// Init Engine
	Termat termat(89, 44);

	// Game Loop
	termat.run([&](Time time, const Inputs &inputs) {
        /// Update

        /// Renderization
        auto [mx, my] = inputs.mouse_pos ();
        termat.setCh (mx, my, L'|', getColor (15, 0));
        // info
        termat.setString (0, 0, getColor (15, 0), "ESC - Exit");
        termat.setString (0, 1, getColor (15, 0), "FPS: {:.1f} | Frames: {} | Mouse: {}, {}", time.FPS (), time.FrameCount (), mx, my);
	});
	return 0;
}