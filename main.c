#include <stdio.h>
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

const char* TITLE = "Automata";
const int WIDTH = 700;
const int HEIGHT = 700;

const int GRID_SIZE = 25;

typedef struct 
{
  int rows;
  int cols;
  bool** grid;
} Board;

Board* init_board(int width, int height)
{
  Board* board = (Board*)malloc(sizeof(Board));
  board->grid = (bool**)malloc(height * sizeof(bool*));
  board->rows = height;
  board->cols = width;
  if (board->grid == NULL) 
  {
    fprintf(stderr, "Failed to Allocate Grid");
  }
  else
  {
    for (int h = 0; h < height; ++h)
    {
      board->grid[h] = (bool*)malloc(width * sizeof(bool));
      if (board->grid[h] == NULL)
      {
        fprintf(stderr, "Failed to Allocate Row");
      } 
      else
      {
        for (int w = 0; w < width; ++w) 
        {
          if (rand() % 2 == 0) 
          {
            board->grid[h][w] = 1;
          }
          else
          {
            board->grid[h][w] = 0;
          }
        }
      }
    }
  }
  return board;
}


Board* init_board_zeros(int width, int height)
{
  Board* board = (Board*)malloc(sizeof(Board));
  board->grid = (bool**)malloc(height * sizeof(bool*));
  board->rows = height;
  board->cols = width;
  if (board->grid == NULL) 
  {
    fprintf(stderr, "Failed to Allocate Grid");
  }
  else
  {
    for (int h = 0; h < height; ++h)
    {
      board->grid[h] = (bool*)malloc(width * sizeof(bool));
      if (board->grid[h] == NULL)
      {
        fprintf(stderr, "Failed to Allocate Row");
      } 
      else
      {
        for (int w = 0; w < width; ++w) 
        {
          board->grid[h][w] = 0;
        }
      }
    }
  }
  return board;
}


Board* copy_board(Board* src, Board* dst)
{
  for (int row = 0; row < src->rows; ++row)
  {
    for (int col = 0; col < src->cols; ++col)
    {
      dst->grid[row][col] = src->grid[row][col];
    }
  }
  return dst;
}


int count_live_neigbours(Board* board, int cellX, int cellY)
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

		if (neighborX >= 0 && neighborX < board->cols && 
			neighborY >= 0 && neighborY < board->rows) 
		{
			if (board->grid[neighborY][neighborX] == 1)
			{
				count++;
			}
		}
	}
  }
  return count;
}


Board* game_loop(Board* board)
{
  Board* temp_board = init_board_zeros(board->rows, board->cols);

  for (int row = 0; row < board->rows; ++row)
  {
    for (int col = 0; col < board->cols; ++col)
    {
      int nbors = count_live_neigbours(board, row, col);
      
      if (board->grid[row][col] == 1)  // Live cell
      {
        if (nbors == 2 || nbors == 3) // Survival
        {
          temp_board->grid[row][col] = 1;
        }
        else 
        {
          temp_board->grid[row][col] = 0;
        }
      }
      else  // Dead cell
      {
        temp_board->grid[row][col] = (nbors == 3);
      }
    }
  }
  board = copy_board(temp_board, board);
  free(temp_board);
  return board;
}

int main() 
{
  srand(time(NULL));
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(2);
  
  Board* board = init_board(GRID_SIZE, GRID_SIZE);
  
  while (!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      int block_size = WIDTH / GRID_SIZE;

      for (int c = 0; c < board->cols; ++c) 
      {
        for (int r = 0; r < board->rows; ++r)
        {
          if (board->grid[r][c] == 1) 
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
    // Leaking Memory? ahh scary (it's fine the OS will clean it)
  }
}
