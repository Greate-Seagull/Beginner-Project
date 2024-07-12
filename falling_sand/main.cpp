#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

int width = 80;
int height = 80;
int cellSize = 10;
int delayFrames = 6;
int maxColor = 7;

vector<vector<int>> frame(width, vector(height, 0));

int getColor(){
    int time = GetTime();
    time /= 5;
    time %= maxColor;
    return time + 1;
}

void chooseColor(int type, Color &color){
    switch(type){
    case 1:
        color = RED;
        break;
    case 2:
        color = ORANGE;
        break;
    case 3:
        color = YELLOW;
        break;
    case 4:
        color = GREEN;
        break;
    case 5:
        color = BLUE;
        break;
    case 6:
        color = MAGENTA;
        break;
    case 7:
        color = PURPLE;
    }
}

int slide(int x, int y){
    if(y - 1 >= 0 && frame[x + 1][y - 1] == 0){
        frame[x + 1][y - 1] = frame[x][y];
        return 1;
    }
    else if(y + 1 < width && frame[x + 1][y + 1] == 0){
        frame[x + 1][y + 1] = frame[x][y];
        return 1;
    }
    return 0;
}

void create(Vector2 position){
    int col = position.x / cellSize;
    int row = position.y / cellSize;

    if(row < 0 || row >= height)
        return;
    if(col < 0 || col >= width)
        return;
    if(frame[row][col] != 0)
        return;

    frame[row][col] = getColor();
}

void update(){
    for(int row = height - 1; row > -1; row--){
        for(int col = 0; col < width; col++){
            if(frame[row][col] == 0)
                continue;
            if(row + 1 >= height)
                continue;

            if(frame[row + 1][col] != 0){
                if(slide(row, col) == 0)
                    continue;
            }
            else{
                frame[row + 1][col] = frame[row][col];
            }

            frame[row][col] = 0;

        }
    }
}

void draw(){
    Color color;
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            if(frame[row][col] == 0)
                continue;

            chooseColor(frame[row][col], color);
            DrawRectangle(col * cellSize, row * cellSize, cellSize, cellSize, color);
        }
    }
}

int main()
{
    InitWindow(width * cellSize, height * cellSize, "Falling Sand");
    SetTargetFPS(60);

    Vector2 mousePos;
    int frames = 0;

    while(WindowShouldClose() == false){
        frames = (frames + 1) % delayFrames;

        switch(frames){
        case 1:
            mousePos = GetMousePosition();
            create(mousePos);
            break;
        case 2:
            create(Vector2{mousePos.x - 1 * cellSize, mousePos.y});
            create(Vector2{mousePos.x + 1 * cellSize, mousePos.y});
            break;
        case 3:
            create(Vector2{mousePos.x - 2 * cellSize, mousePos.y});
            create(Vector2{mousePos.x + 2 * cellSize, mousePos.y});
            break;
        }

        update();

        BeginDrawing();

        ClearBackground(BLACK);
        draw();

        EndDrawing();

    }

    CloseWindow();

    return 0;
}
