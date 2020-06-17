#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <memory>
#include <unordered_map>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  int grid_width;
  int grid_height; 

 private:
  //Added unique pointers for snake, food and distractors
  std::unique_ptr<Snake> snake = std::make_unique<Snake>(grid_width,grid_height);

  std::unique_ptr<SDL_Point> food = std::make_unique<SDL_Point>();

  std::unique_ptr<SDL_Point[]> distractors = std::make_unique<SDL_Point[]>(5);

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool running{true};
  void PlaceFood();
  void Update();
  void PlaceDistractors();

  // Keeps track so that the distractors and food dont be in the same position
  std::unordered_map<std::string,std::string> _isBlocked;

};

#endif