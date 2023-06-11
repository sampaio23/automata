#include <time.h>

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 50

#define ROWS ( SCREEN_HEIGHT / CELL_SIZE )
#define COLS ( SCREEN_WIDTH / CELL_SIZE ) 

typedef enum {
	CELL_DEAD = 0,
	CELL_ALIVE = 1,
} Cell;

const Color cell_color[] = {
	BLACK,
	WHITE,
};

typedef struct {
    Cell cells[ROWS][COLS];
} Board;

Board generate_board(void)
{
	Board board;

	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			board.cells[y][x] = GetRandomValue(CELL_DEAD, CELL_ALIVE);
		}
	}

	return board;
}

int main(void) {
	SetRandomSeed(time(0));
	Board board = generate_board();

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
	BeginDrawing();
		{

		for (int y = 0; y < ROWS; y++) {
			for (int x = 0; x < COLS; x++) {
				DrawRectangle(x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, cell_color[board.cells[y][x]]);
			}
		}
			ClearBackground(BLACK);
			DrawFPS(10, 10);
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
