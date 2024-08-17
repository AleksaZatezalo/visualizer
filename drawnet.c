#include "drawnet.h"
#define WINDOW_X    10
#define WINDOW_Y    -20
#define WINDOW_WIDTH    1770
#define WINDOW_HEIGHT 1410

#define NETWORK_WIDTH 1600
#define NETWORK_HEIGHT 1300

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

int arch[] = {2, 2, 1};


void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for(int w = 0; w < radius * 2; w++) {
        for(int h = 0; h < radius * 2; h++){
            int dx = radius - w;
            int dy = radius - h;
            if((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x+dx, y+dy);
            }
        }
    }
}

void render_network(SDL_Renderer *renderer, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x55, 0x55, 255);
    SDL_Rect box;
    box.w = NETWORK_WIDTH;
    box.h = NETWORK_HEIGHT;
    box.x = x;
    box.y = y;

    SDL_RenderDrawRect(renderer, &box);

    SDL_Color c;
    c.r = 0x00;
    c.g = 0xff;
    c.b = 0x00;
    c.a = 0x00;

    int layers = ARRAY_LEN(arch);
    int layer_width = NETWORK_WIDTH / layers;
    int x_offset = layer_width / 2;

    for (int i = 0; i < layers; i++){
        int xPos = x + i * layer_width + x_offset;
        for(int j = 0; j < arch[i]; j++) {
            int neuron_height = NETWORK_HEIGHT / arch[i];
            int y_offset = neuron_height / 2;
            int yPos = y + j * neuron_height + y_offset;

            if(i < layers){
                for(int k = 0; k < arch[i+i]; k++){
                    int xPos2 = x + (i+1) * layer_width + x_offset;        
                    int neuron_height2 = NETWORK_HEIGHT / arch[i+1];
                    int y_offset2 = neuron_height2 / 2;
                    int yPos2 = y + k * neuron_height2 + y_offset2;

                    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);
                    SDL_RenderDrawLine(renderer, xPos, yPos, xPos2, yPos2);
                }
            }

            SDL_RenderFillCircle(renderer, xPos, yPos, 80, c);
        }
    }

    return;
}

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_INIT_VIDEO < 0 ){
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
    }

    window = SDL_CreateWindow(
        "",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if(!window){
        fprintf(stderr, "ERROR: !window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer){
        fprintf(stderr, "ERROR: !renderer");
    }

    int net_x = (WINDOW_WIDTH/2) - (NETWORK_WIDTH/2);
    int net_y = (WINDOW_HEIGHT/2) - (NETWORK_HEIGHT/2);

    SDL_Event event;
    bool quit = false;
    while(!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                    break;
            }
        }
        SDL_RenderClear(renderer);
        //START RENDER LOOP

        render_network(renderer, net_x, net_y);

        //END RENDER LOOP
        SDL_SetRenderDrawColor(renderer, 0x11,0x11,0x11,100);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}