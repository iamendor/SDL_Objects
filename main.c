#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    double x;
    double y;
}Point;

const double eps = 1.2;


const Point h20k40 = { 320, 240 };
const Point k40k0 = {240, 200};
const Point k40k40 = {240, 240};
const Point n0k80 = {400, 280};
const Point n0k40 = {400, 240};
const int fw = 11, fh = 10;

const int chars[26][11] = { 
/*A*/   { 112, 216, 396, 774, 1799, 2047, 1799, 1799, 1799, 1799 },
/*B*/   { 2046, 2047, 1795, 1795, 2046, 1795, 1795, 2047, 2046 },
        {0},{0},
/*E*/   { 2047, 2047, 1792, 1792, 2032, 2032, 1792, 1792, 2047, 2047 },
        {0},{0},
/*H*/   { 1799, 1799, 1799, 1799, 2047, 2047, 1799, 1799, 1799, 1799 },
/*I*/   { 2047, 2047, 112, 112, 112, 112, 112, 112, 2047, 2047 },
        {0},
/*K*/   { 1799, 1806, 1820, 1848, 2032, 2032, 1848, 1820, 1806, 1799 },
/*L*/   { 1792, 1792, 1792, 1792, 1792, 1792, 1792, 1792, 2047, 2047 },
        {0},{0},
/*O*/   { 508, 990, 1799, 1799, 1799, 1799, 1799, 990, 508 },
/*P*/   { 2046, 2047, 1795, 1795, 2046, 2044, 1792, 1792, 1792, 1792 },
        {0},
/*R*/   { 2044, 2047, 1799, 2046, 2040, 1848, 1820, 1806, 1799 },
/*S*/   { 2047, 2047, 1792, 1792, 2047, 2047, 7, 7, 2047, 2047 },
        {0},{0},{0},{0},{0}, {0},
/*Z*/   { 2047, 2047, 7, 28, 112, 448, 1792, 1792, 2047, 2047 },
};




double calcDistance(Point p1, Point p2){
    return sqrt(pow(p2.x - p1.x, 2)+pow(p2.y-p1.y, 2));
}

void renderText(SDL_Renderer *renderer, char *str, int x, int y){
    int len = strlen(str);

    for(int i=0; i<len; i++){
        char c = str[i];
        int index = c - 'A';

        for(int j=0; j<10; j++){
            for(int k=0; k<11; k++){
                if (chars[index][j] & (1 << (10 - k))) pixelRGBA(renderer, x+k, y+j, 255, 255, 255, 255);
            }
        }
        x += 12;
    }
}


int main()
{


    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    SDL_Window *window = SDL_CreateWindow( "Szorg3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,640, 480, SDL_WINDOW_SHOWN);
    if(!window){
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for(int y=0; y<=480; y++){
        for(int x=0; x<=640; x++){
            Point p = {x, y};
            double d1 = calcDistance(p, h20k40);
            double d2 = calcDistance(p, k40k0) + calcDistance(p, n0k80);
            double d3 =  calcDistance(p, k40k40) - calcDistance(p, n0k40);
            double d4 =  calcDistance(p, h20k40);
            double d5 =  calcDistance(p, (Point) {400, y});
            if(fabs(d1 - 200.0) < eps) pixelRGBA(renderer, x, y, 255, 0, 0, 255);
            else if(fabs(d2 - 250) < eps) pixelRGBA(renderer, x, y, 0, 255, 0, 255);
            else if(fabs(d3 - 100) < eps || fabs(d3 + 100) < eps /* Ezzel teljes hiperbola */) pixelRGBA(renderer, x, y, 0, 0, 255, 255);
            else if(fabs(d4 - d5) < eps) pixelRGBA(renderer, x, y, 255, 255, 255, 255);
        }
    }
    renderText(renderer, "KOR", 20, 100);
    renderText(renderer, "ELLIPSZIS", 20, 120);
    renderText(renderer, "HIPERBOLA", 20, 140);
    renderText(renderer, "PARABOLA", 20, 160);
    SDL_RenderPresent(renderer);


    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
