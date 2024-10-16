/*
 The MIT License (MIT)
 
 Copyright (c) 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */


//#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "display.h"
#include "graphics.h"


static bool isRunning = true;
static SDL_Window *window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Event event;


extern void START(void);

int hpprgm_thread(void* data) {
    START();
    isRunning = false;
    return 0;
}

int main(int argc, const char * argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
//    window = SDL_CreateWindow("SDL3 Project",LCD_WIDTH_PX, LCD_HEIGHT_PX, SDL_WINDOW_HIGH_PIXEL_DENSITY); // SDL3
    window = SDL_CreateWindow("SDL3 Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LCD_WIDTH_PX * 2, LCD_HEIGHT_PX * 2, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
//    renderer = SDL_CreateRenderer(window, NULL);
    renderer = SDL_CreateRenderer(window, -1, NULL);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    
    texture = SDL_CreateTexture(
                                renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                LCD_WIDTH_PX,
                                LCD_HEIGHT_PX
                                );
    if (!texture) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 4;
    }
//    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    
    
    SDL_Thread* thread = SDL_CreateThread( hpprgm_thread, "HPPRGM Thread",  NULL );
    const int pitch = LCD_WIDTH_PX * sizeof(Uint32);
    
    while(isRunning) {
        if (SDL_UpdateTexture(texture, NULL, displayRAM(), pitch) != 0) {
            std::cout << "SDL_UpdateTexture failed: " << SDL_GetError() << '\n';
        }
        
        if (SDL_RenderClear(renderer) != 0) {
            std::cout << "SDL_RenderClear failed: " << SDL_GetError() << '\n';
        }
//        SDL_RenderTexture(renderer, texture, NULL, NULL); // SDL3
        SDL_RenderCopy(renderer, texture, NULL, NULL); // SDL2
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16);
        SDL_PollEvent(&event);
        
// SDL3
#define SDL_EVENT_QUIT              SDL_QUIT
#define SDL_EVENT_MOUSE_BUTTON_DOWN SDL_MOUSEBUTTONDOWN
#define SDL_EVENT_MOUSE_BUTTON_UP   SDL_MOUSEBUTTONUP
#define SDL_EVENT_MOUSE_MOTION      SDL_MOUSEMOTION
#define SDL_EVENT_KEY_DOWN          SDL_KEYDOWN
        
        if (SDL_EVENT_QUIT == event.type) break;
        
        switch ( event.type ) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_UP:
                break;
                
            case SDL_EVENT_MOUSE_MOTION:
                break;
                
            case SDL_EVENT_KEY_DOWN:
                break;
                
        }
    }

    SDL_DetachThread(thread);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}




