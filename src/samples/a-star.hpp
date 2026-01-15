// AStarViz.cpp
#include <termat/api.hpp>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include <format>
#include <algorithm> // Necessário para std::reverse

class AStar : public termat::api::Application {
public:
    AStar()
        : Application(128, 64, L"A* Visualizer", 8, 8) {}

    void onInit() override {
        std::srand((unsigned)std::time(nullptr));
        reset();
    }

    void onUpdate(double dt) override {
        acc += dt;
        
        // 1. Lógica de RESET após terminar tudo
        if (!running && !animatingPath && (finishedTimer > 0.0)) {
            finishedTimer -= dt;
            if (finishedTimer <= 0.0) reset();
            return;
        }

        // 2. Lógica de ANIMAÇÃO DO CAMINHO (Novo)
        if (animatingPath) {
            pathAnimTimer += dt;
            if (pathAnimTimer >= pathAnimDelay) {
                pathAnimTimer = 0.0;
                
                // Se ainda tem passos para desenhar no caminho
                if (pathDrawCount < pathIndices.size()) {
                    int idx = pathIndices[pathDrawCount];
                    // Só pinta se não for Start nem Goal (para manter as cores originais deles)
                    if (cellAtIdx(idx) != START_C && cellAtIdx(idx) != GOAL_C) {
                        setCell(idx, PATH);
                    }
                    pathDrawCount++;
                } else {
                    // Terminou de desenhar o caminho
                    animatingPath = false;
                    finishedTimer = 2.0; // Agora sim espera para resetar
                }
            }
            return; // Não executa o passo do A* enquanto anima o caminho
        }

        // 3. Lógica do ALGORITMO A*
        stepTimer += dt;
        if (running && stepTimer >= stepDelay) {
            stepTimer = 0.0;
            stepAstarOnce();
        }
    }

    void onRender() override {
        auto [w, h] = getWindowSize();
        // Clear background
        gfx2D.FillRect(0, 0, w, h, termat::gfx::PIXEL_SOLID, termat::gfx::FG_BLACK | termat::gfx::BG_BLACK);
        gfx2D.Rect(marginX - 1, marginY - 1, marginX + gridW * cellSize + 1, marginY + gridH * cellSize + 1, termat::gfx::PIXEL_SOLID, termat::gfx::FG_GREY);

        // Draw grid cells
        for (int gy = 0; gy < gridH; ++gy) {
            for (int gx = 0; gx < gridW; ++gx) {
                int px = marginX + gx * cellSize;
                int py = marginY + gy * cellSize;

                CellType t = cellAtXY(gx, gy);
                short attr = attrForType(t);
                
                // fill cell with pixels (simple)
                for (int yy = 0; yy < cellSize; ++yy) {
                    for (int xx = 0; xx < cellSize; ++xx) {
                        gfx2D.Pixel(px + xx, py + yy, termat::gfx::PIXEL_SOLID, attr);
                    }
                }
            }
        }

        // Draw path overlays (thin) - MODIFICADO
        // Só desenha até o pathDrawCount (efeito visual progressivo dos pontos centrais)
        if (!pathIndices.empty() && (animatingPath || !running)) {
            // Desenha apenas até onde a animação chegou (pathDrawCount)
            // Se já terminou a animação (!running e !animatingPath), desenha tudo.
            size_t limit = animatingPath ? pathDrawCount : pathIndices.size();
            
            for (size_t i = 0; i < limit; ++i) {
                int idx = pathIndices[i];
                int gx = idx % gridW;
                int gy = idx / gridW;
                int px = marginX + gx * cellSize;
                int py = marginY + gy * cellSize;
                // draw center pixel to emphasize
                gfx2D.Pixel(px + cellSize/2, py + cellSize/2, termat::gfx::PIXEL_SOLID, termat::gfx::FG_MAGENTA);
            }
        }

        // small legend on right
        int lx = marginX + gridW * cellSize + 2;
        int ly = marginY;
        drawLegend(lx, ly, 2);
    }

    void onShutdown() override {}

private:
    enum CellType : uint8_t { EMPTY=0, WALL, OPEN, CLOSED, PATH, START_C, GOAL_C, CURRENT };

    struct Node {
        int idx;
        int g;
        int f;
        Node(int i=0,int gg=0,int ff=0):idx(i),g(gg),f(ff){}
        bool operator<(Node const& o) const { return f > o.f; } 
    };

    // CONFIG
    const int gridW = 56;
    const int gridH = 30;
    const int cellSize = 2;        
    const int marginX = 1;
    const int marginY = 1;
    const double stepDelay = 0;  // Velocidade da busca A*
    const int wallChancePercent = 25; 

    // CONFIG ANIMAÇÃO CAMINHO
    const double pathAnimDelay = 0.02; // Segundos entre cada quadrado do caminho (ajuste para mais rápido/lento)

    // STATE
    double acc = 0.0;
    double stepTimer = 0.0;
    bool running = true;
    
    // Estados de animação
    bool animatingPath = false;
    size_t pathDrawCount = 0;
    double pathAnimTimer = 0.0;
    double finishedTimer = 0.0;

    std::vector<uint8_t> grid; 
    int startIdx = 0;
    int goalIdx = 0;

    std::vector<int> gScore;      
    std::vector<int> cameFrom;    
    std::priority_queue<Node> openQueue;
    std::vector<char> inClosed;   
    std::vector<char> inOpen;     

    std::vector<int> pathIndices;

    int steps = 0;

    // helpers
    inline int idxAt(int x,int y) const { return y * gridW + x; }
    inline bool inBounds(int x,int y) const { return x>=0 && x<gridW && y>=0 && y<gridH; }
    inline CellType cellAtXY(int x,int y) const { return (CellType)grid[y*gridW + x]; }
    inline CellType cellAtIdx(int idx) const { return (CellType)grid[idx]; }
    inline void setCell(int idx, CellType t) { grid[idx] = (uint8_t)t; }
    inline short attrForType(CellType t) const {
        using namespace termat::gfx;
        switch(t) {
            case WALL:    return FG_GREY | BG_BLACK;
            case OPEN:    return FG_YELLOW | BG_BLACK;
            case CLOSED:  return FG_GREEN | BG_BLACK;
            case PATH:    return FG_MAGENTA | BG_BLACK;
            case START_C: return FG_CYAN | BG_BLACK;
            case GOAL_C:  return FG_RED | BG_BLACK;
            case CURRENT: return FG_WHITE | BG_BLACK;
            case EMPTY:
            default:      return FG_BLACK | BG_BLACK;
        }
    }

    void reset() {
        running = true;
        animatingPath = false; // Reset flag animação
        pathDrawCount = 0;
        pathAnimTimer = 0.0;
        finishedTimer = 0.0;
        steps = 0;
        grid.assign(gridW * gridH, (uint8_t)EMPTY);

        // random walls
        for (int y = 0; y < gridH; ++y) {
            for (int x = 0; x < gridW; ++x) {
                if (std::rand()%100 < wallChancePercent) {
                    setCell(idxAt(x,y), WALL);
                }
            }
        }

        startIdx = pickEmptyCell();
        setCell(startIdx, START_C);
        goalIdx = pickEmptyCell();
        while (goalIdx == startIdx) goalIdx = pickEmptyCell();
        setCell(goalIdx, GOAL_C);

        int N = gridW * gridH;
        gScore.assign(N, (std::numeric_limits<int>::max)());
        cameFrom.assign(N, -1);
        inClosed.assign(N, 0);
        inOpen.assign(N, 0);
        while (!openQueue.empty()) openQueue.pop();
        pathIndices.clear();

        gScore[startIdx] = 0;
        openQueue.push(Node(startIdx, 0, heuristic(startIdx, goalIdx)));
        inOpen[startIdx] = 1;
        steps = 0;
    }

    int pickEmptyCell() {
        int N = gridW * gridH;
        for (int attempt = 0; attempt < 1000; ++attempt) {
            int i = std::rand() % N;
            if (grid[i] == EMPTY) return i;
        }
        for (int i = 0; i < N; ++i) if (grid[i] == EMPTY) return i;
        return 0;
    }

    int heuristic(int a, int b) const {
        int ax = a % gridW, ay = a / gridW;
        int bx = b % gridW, by = b / gridW;
        return std::abs(ax - bx) + std::abs(ay - by);
    }

    void stepAstarOnce() {
        if (!running) return;
        if (openQueue.empty()) {
            running = false;
            finishedTimer = 1.4; 
            pathIndices.clear();
            return;
        }

        Node cur = openQueue.top(); openQueue.pop();
        int curIdx = cur.idx;

        if (cur.f - cur.g != heuristic(curIdx, goalIdx)) {
            return;
        }

        // SE ENCONTROU O OBJETIVO
        if (curIdx == goalIdx) {
            reconstructPath(curIdx);
            running = false; // Para o cálculo do A*
            animatingPath = true; // Inicia a animação do caminho
            pathDrawCount = 0;
            return;
        }

        inClosed[curIdx] = 1;
        if (cellAtIdx(curIdx) == EMPTY) setCell(curIdx, CLOSED);

        static const int dir[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        int cx = curIdx % gridW, cy = curIdx / gridW;
        for (int d = 0; d < 4; ++d) {
            int nx = cx + dir[d][0];
            int ny = cy + dir[d][1];
            if (!inBounds(nx, ny)) continue;
            int nidx = idxAt(nx, ny);
            if (inClosed[nidx]) continue;
            if (cellAtIdx(nidx) == WALL) continue;

            int tentative_g = gScore[curIdx] + 1;
            if (tentative_g < gScore[nidx]) {
                cameFrom[nidx] = curIdx;
                gScore[nidx] = tentative_g;
                int f = tentative_g + heuristic(nidx, goalIdx);
                openQueue.push(Node(nidx, tentative_g, f));
                inOpen[nidx] = 1;
                if (cellAtIdx(nidx) == EMPTY) setCell(nidx, OPEN);
            }
        }
        steps++;
    }

    void reconstructPath(int curIdx) {
        pathIndices.clear();
        int cur = curIdx;
        while (cur != -1) {
            pathIndices.push_back(cur);
            // REMOVI: setCell(cur, PATH); -> Isso agora acontece no onUpdate gradualmente
            cur = cameFrom[cur];
        }
        // Inverte para desenhar do Inicio -> Fim (fica mais bonito visualmente)
        std::reverse(pathIndices.begin(), pathIndices.end());
    }

    void drawLegend(int x, int y, int box_size) {
        using namespace termat::gfx;
        gfx2D.FillRect(x, y + 0, x + box_size, y + 0 + box_size,  PIXEL_SOLID, FG_CYAN | BG_BLACK); gfx2D.String(x + 1 + box_size, y + 0, L"Start");
        gfx2D.FillRect(x, y + 3, x + box_size, y + 3 + box_size,  PIXEL_SOLID, FG_RED  | BG_BLACK); gfx2D.String(x + 1 + box_size, y + 3, L"Goal");
        gfx2D.FillRect(x, y + 6, x + box_size, y + 6 + box_size,  PIXEL_SOLID, FG_GREY | BG_BLACK); gfx2D.String(x + 1 + box_size, y +6, L"Wall");
        gfx2D.FillRect(x, y + 9, x + box_size, y + 9 + box_size,  PIXEL_SOLID, FG_YELLOW | BG_BLACK); gfx2D.String(x + 1 + box_size, y +9, L"Open");
        gfx2D.FillRect(x, y + 12,x + box_size, y + 12 + box_size, PIXEL_SOLID, FG_GREEN  | BG_BLACK); gfx2D.String(x + 1 + box_size, y +12, L"Closed");
        gfx2D.FillRect(x, y + 15,x + box_size, y + 15 + box_size, PIXEL_SOLID, FG_MAGENTA| BG_BLACK); gfx2D.String(x + 1 + box_size, y +15, L"Path");
    }
};