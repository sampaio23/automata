#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 100

#define ROWS ( SCREEN_HEIGHT / CELL_SIZE )
#define COLS ( SCREEN_WIDTH / CELL_SIZE ) 

#define MOD(a,b) ((a % b + b) % b)

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

int cell_neighbours(Board board, int y, int x)
{
	int neighbours = 0;

	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (dx != 0 || dy != 0) {
				neighbours += board.cells[MOD((y + dy),ROWS)][MOD((x + dx),COLS)];
			}
		}
	}

	return neighbours;
}

int main(void) {
	SetRandomSeed(time(0));
	Board board = generate_board();
	Board next_board = { 0 };

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");

	int framesCounter = 0;
	int framesSpeed = 4;
	bool paused = false;
	bool step = false;
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_SPACE)) paused = !paused;

		if (paused) {
			Vector2 mouse_position = GetMousePosition();
			int cell_x = mouse_position.x / CELL_SIZE;
			int cell_y = mouse_position.y / CELL_SIZE;
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				board.cells[cell_y][cell_x] = CELL_ALIVE;
			} else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
				board.cells[cell_y][cell_x] = CELL_DEAD;
			}

			if (IsKeyPressed(KEY_ENTER)) step = true;
		}

		BeginDrawing();
		{
			for (int y = 0; y < ROWS; y++) {
				for (int x = 0; x < COLS; x++) {
					DrawRectangle(x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, cell_color[board.cells[y][x]]);
				}
			}

			if (paused) {
				DrawText("paused", SCREEN_WIDTH - 100, SCREEN_HEIGHT - 30, 20, RED);
			}
		}
		EndDrawing();
	
		framesCounter++;

		if (((framesCounter >= (60/framesSpeed)) && !paused) || step) {
			for (int y = 0; y < ROWS; y++) {
				for (int x = 0; x < COLS; x++) {
					int neighbours = cell_neighbours(board, y, x);
					switch (board.cells[y][x]) {
						case CELL_DEAD:
							if (neighbours == 3) {
								next_board.cells[y][x] = CELL_ALIVE;
							} else {
								next_board.cells[y][x] = CELL_DEAD;
							}
							break;
						case CELL_ALIVE:
							if (neighbours == 2 || neighbours == 3) {
								next_board.cells[y][x] = CELL_ALIVE;
							} else {
								next_board.cells[y][x] = CELL_DEAD;
							}
							break;
						default:
							fprintf(stderr, "ERROR: unreachable");
							exit(1);
					}
				}
			}

			board = next_board;
			framesCounter = 0;
			step = false;
		}
	}

	CloseWindow();

	return 0;
}

