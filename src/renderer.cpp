#include "renderer.h"
#include <iostream>
#include <string>
#include <thread>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake* snake, SDL_Point* food, SDL_Point* distractors) {
  
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  
  // Draw boundaries
  SDL_Rect side;
  side.w = block.w/4;
  side.h = screen_height;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  side.x = 0;
  side.y = 0;
  SDL_RenderFillRect(sdl_renderer,&side);
  side.w = screen_width;
  side.h = block.h/4;
  SDL_RenderFillRect(sdl_renderer,&side);
  side.x = 0;
  side.y = (grid_height)*block.h-block.h/4;
  SDL_RenderFillRect(sdl_renderer,&side);
  side.x = (grid_width)*block.w-block.w/4;
  side.y = 0;
  side.w = block.w/4;
  side.h = screen_height;
  SDL_RenderFillRect(sdl_renderer,&side);

  // Draw Distractors
  for(int i=0;i<5;i++)
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = distractors[i].x * block.w;
    block.y = distractors[i].y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect insBlock;
    insBlock.w = block.w/10;
    insBlock.h = block.h/10;
    insBlock.x = distractors[i].x * block.w + block.w/2;
    insBlock.y = distractors[i].y * block.h + block.h/2;
    SDL_RenderFillRect(sdl_renderer,&insBlock);
  }
  
  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);

  block.x = food->x * block.w;
  block.y = food->y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake->body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake->head_x) * block.w;
  block.y = static_cast<int>(snake->head_y) * block.h;
  if (snake->alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int &score, int& fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
