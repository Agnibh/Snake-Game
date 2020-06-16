#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  _isBlocked.emplace(std::to_string(prev_head_cell.x*32+prev_head_cell.y),"blocked");
 
  if (!growing)
  {
    // Remove the tail from the vector.
    _isBlocked.erase(std::to_string((*body.begin()).x*32+(*body.begin()).y));
    body.erase(body.begin());
  } 
  else 
  {
    growing = false;
    size++;
  }

  if(_isBlocked.find(std::to_string(current_head_cell.x*32+current_head_cell.y))!=_isBlocked.end())
  {
    alive=false;
  }
  
  if(current_head_cell.x==0||current_head_cell.x==grid_width-1||current_head_cell.y==0||current_head_cell.y==grid_height-1)
    alive=false;
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int& x, int& y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }

  if(_isBlocked.find(std::to_string(x*32+y))!=_isBlocked.end())
    return true;

  return false;
}