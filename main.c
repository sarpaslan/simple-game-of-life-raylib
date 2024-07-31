#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define CAMERA_SPEED 800
// TODO Add row / col
#define GRID_COUNT 100

Vector2 Vector2Normalize(Vector2 v); // from raymath.h
void LoadGameGrid();
void HandleGameLogic();
void DrawGameGrid();
void HandleCameraMovement(Camera2D *camera);
void PlaceShip(int position);
void PlaceBlinker(int position);
void PlaceE(int position);
void PlaceGlider(int position);
int CountLiveNeighbourCell(int a[GRID_COUNT][GRID_COUNT], int x, int y);

bool pause = true;
const int GRID_SIZE = 100;
int grid[GRID_COUNT][GRID_COUNT];
int generation = 0;

int main(void)
{
    const int screenWidth = 900;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "game of life");
    Camera2D camera = {0};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.1;
    SetTargetFPS(0);
    LoadGameGrid();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        HandleCameraMovement(&camera);
        if (!pause)
            HandleGameLogic();
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;
        DrawGameGrid();
        EndMode2D();
        DrawFPS(5, 5);
        DrawRectangle(10, screenHeight - 50, 200, 40, BLACK);
        DrawText(TextFormat("Generation: %d", generation), 10, screenHeight - 40, 25, GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
void DrawGameGrid()
{
    // rendering
    for (int x = 0; x < GRID_COUNT; x++)
    {
        for (int y = 0; y < GRID_COUNT; y++)
        {
            if (grid[x][y] == 1)
            {
                DrawRectangle(x * GRID_SIZE,
                              y * GRID_SIZE,
                              GRID_SIZE, GRID_SIZE, WHITE);
            }
            else
            {
                DrawRectangleLines(x * GRID_SIZE,
                                   y * GRID_SIZE,
                                   GRID_SIZE, GRID_SIZE, GRAY);
            }
        }
    }
}

void HandleGameLogic()
{
    int alive = 0;
    int temp_grid[GRID_COUNT][GRID_COUNT];
    for (int i = 0; i < GRID_COUNT; i++)
    {
        for (int j = 0; j < GRID_COUNT; j++)
        {
            alive = CountLiveNeighbourCell(grid, i, j);

            if (grid[i][j] == 1 && (alive == 2 || alive == 3))
            {
                temp_grid[i][j] = 1;
            }
            else if (grid[i][j] == 0 && alive == 3)
            {
                temp_grid[i][j] = 1;
            }
            else
            {
                temp_grid[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < GRID_COUNT; i++)
    {
        for (int j = 0; j < GRID_COUNT; j++)
        {
            grid[i][j] = temp_grid[i][j];
        }
    }
    generation++;
}

int CountLiveNeighbourCell(int a[GRID_COUNT][GRID_COUNT], int x, int y)
{
    int count = 0;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            // Wrap around the screen
            int col = (x + i + GRID_COUNT) % GRID_COUNT;
            int row = (y + j + GRID_COUNT) % GRID_COUNT;
            count += grid[col][row];
        }
    }
    count -= grid[x][y];
    return count;
}

void HandleCameraMovement(Camera2D *camera)
{
    Vector2 velocity = {0, 0};
    if (IsKeyDown(KEY_W))
        velocity.y -= 1;
    if (IsKeyDown(KEY_S))
        velocity.y += 1;
    if (IsKeyDown(KEY_A))
        velocity.x -= 1;
    if (IsKeyDown(KEY_D))
        velocity.x += 1;
    velocity = Vector2Normalize(velocity);
    camera->target.x += velocity.x * CAMERA_SPEED * GetFrameTime();
    camera->target.y += velocity.y * CAMERA_SPEED * GetFrameTime();
}

void LoadGameGrid()
{
    for (int x = 0; x < GRID_COUNT; x++)
    {
        for (int y = 0; y < GRID_COUNT; y++)
        {
            grid[x][y] = 0; // rand() % 2;
        }
    }
    PlaceE(20);
    PlaceE(50);
}
// Testing patterns

void PlaceBlinker(int position)
{
    grid[position][position] = 1;
    grid[position][position + 1] = 1;
    grid[position][position + 2] = 1;
}
void PlaceE(int position)
{
    grid[position][position + 1] = 1;
    grid[position][position] = 1;
    grid[position][position + 2] = 1;
    grid[position - 1][position + 1] = 1;
    grid[position + 1][position + 2] = 1;
}
void PlaceGlider(int position)
{
    grid[position][position] = 1;
    grid[position][position + 1] = 1;
    grid[position][position + 2] = 1;
    grid[position - 1][position + 2] = 1;
    grid[position - 2][position + 1] = 1;
}

void PlaceShip(int position)
{
    grid[position][position + 1] = 1;
    grid[position][position] = 1;
    grid[position][position + 2] = 1;
    grid[position - 1][position + 1] = 1;
    grid[position + 1][position + 2] = 1;
}
