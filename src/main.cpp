#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <memory>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // Added unique pointers for renderer, controller and game
  std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(kScreenWidth,kScreenHeight,kGridWidth, kGridHeight);

  std::unique_ptr<Controller> controller = std::make_unique<Controller>();

  std::unique_ptr<Game> game = std::make_unique<Game>(kGridWidth, kGridHeight);

  game->Run(std::move(controller), std::move(renderer), kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game->GetScore() << "\n";
  std::cout << "Size: " << game->GetSize() << "\n";
  return 0;
}