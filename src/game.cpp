#include "game.h"
#include <iostream>
#include "SDL.h"
#include <thread>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    //: snake(grid_width, grid_height),
    : grid_width(grid_width),
      grid_height(grid_height),
      engine(dev()),
      random_w(1, static_cast<int>(grid_width)-2),    //changed the range to  1 less
      random_h(1, static_cast<int>(grid_height)-2) {
  PlaceFood();
  PlaceDistractors();
  
}

//void Game::Run(Controller const &controller, Renderer &renderer,
void Game::Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  //bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller->HandleInput(running, snake.get());
    Update();
    renderer->Render(snake.get(), food.get(), distractors.get());

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer->UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food->x = x;
      food->y = y;
      _isBlocked.emplace(std::to_string(x*32+y),"blocked");
      return;
    }
  }
}

void Game::PlaceDistractors()
{
  for(int i=0;i<5;)
  {
    int x = random_w(engine);
    int y = random_h(engine);
    if(_isBlocked.find(std::to_string(x*32+y))!=_isBlocked.end())
      continue;
    distractors[i].x=x;
    distractors[i].y=y;
    _isBlocked.emplace(std::to_string(x*32+y),"blocked");
    i++;
  }
}

void Game::Update() {
  if (!snake->alive)
  {
    return;
  } 
    
  snake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food->x == new_x && food->y == new_y) {
    score++;
    _isBlocked.clear();
    PlaceFood();
    PlaceDistractors();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }