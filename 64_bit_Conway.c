#include <stdio.h>
#include "raylib.h"
#include <time.h>
#include <stdlib.h>

const char* TITLE = "Automata";
const int WIDTH = 700;
const int HEIGHT = 700;

const int GRID_SIZE = 8;

typedef struct 
{
  int rows;
  int cols;
  uint64_t grid;
} Board;

Board init_board_random(int width, int height)
{
  Board board = {0};
  board.rows = height;
  board.cols = width;
  board.grid = 0;

  for (int h = 0; h < height; ++h)
  {
    for (int w = 0; w < width; ++w) 
    {
      if (rand() % 2 == 1) 
      {
        board.grid |= (1ULL << (h * width + w));
      }
    }
  }
  return board;
}

Board init_board_seed(int width, int height, uint64_t seed)
{
  Board board = {0};
  board.rows = height;
  board.cols = width;
  board.grid = seed;
  return board;
}


bool is_set(Board board, int cellX, int cellY)
{
  return board.grid & (1ULL << (cellX * board.cols + cellY));
}

int count_live_neigbours(Board board, int cellX, int cellY)
{
  int count = 0;
  
  for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0)
				continue;

			int neighborX = cellX + dx;
			int neighborY = cellY + dy;

			if (neighborX >= 0 && neighborX < board.cols && 
				neighborY >= 0 && neighborY < board.rows) 
			{
				if (is_set(board, neighborX, neighborY))
				{
					count++;
				}
			}
		}
	}
  return count;
}


Board game_loop(Board board)
{
  Board temp_board = {board.rows, board.cols, 0};

  for (int row = 0; row < board.rows; ++row)
  {
    for (int col = 0; col < board.cols; ++col)
    {
      int nbors = count_live_neigbours(board, row, col);
          
      if (is_set(board, col, row)) // Live Cell 
      {
        if (nbors == 2 || nbors == 3) // Survival
        {
          temp_board.grid |= (1ULL << (row * board.cols + col));
        }
        else 
        {
          temp_board.grid &= ~(1ULL << (row * board.cols + col));
        }
      }
      else  // Dead cell
      {
        if (nbors == 3)
        {
          temp_board.grid |= (1ULL << (row * board.cols + col));
        }
      }
    }
  }
  return temp_board;
}

int main() 
{
  srand(time(NULL));
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(1);
  
  Board board = init_board_seed(GRID_SIZE, GRID_SIZE, 9060217272139776);
  
  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      int block_size = WIDTH / GRID_SIZE;

      for (int c = 0; c < board.cols; ++c) 
      {
        for (int r = 0; r < board.rows; ++r)
        {
          if (is_set(board, c, r)) 
          { 
            DrawRectangle(c * block_size, r * block_size, block_size, block_size, PURPLE);
          } 
          else 
          {
            DrawRectangleLines(c * block_size, r * block_size, block_size, block_size, WHITE);
          }
        }
      }
      board = game_loop(board);
    EndDrawing();
  }
}
