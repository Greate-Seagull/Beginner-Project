#include <iostream>
#include <cstring>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {150, 200, 90, 255};
Color darkGreen = {40, 50, 25, 255};

int cellSize = 25;
int windowCellCount = 36;

double lastUpdateTime = 0;

class Food{
protected:
    Vector2 position;
    Image image;
    Texture2D texture;

public:
    Food();
    ~Food();
    void Draw();
    void GenerateRandomPos(int start, int end);
    void GenerateRandomImage();
    Vector2 GetPosition();
};

class Snake{
private:
    deque<Vector2> body;
    Vector2 direction;
public:
    Snake();
    void Draw();
    void Move(const Vector2 &direction);
    void Grow();
    void BackHome();
    Vector2 GetDirection();
    deque<Vector2> GetBody();
};

class Game{
private:
    Food* food;
    Snake* snake;

    int running;
    int score;
    float mode;
    static int highestScore;

    Vector2 inputDirection;
public:
    Game();
    ~Game();

    void Update();
    int IsFoodCollided();
    int IsSnakeCollided();
    void Draw();
    void GameOver();
    void GameReset();
    void Pause();
    void InputDirection();
    void StartGame();
    int GetScore();
    int GetHighestScore();
    void changeMode(float mode);
    int EventTriggered();
    int IsRunning();

    static const Rectangle lineBox;
    static const int offsetCellCount;
    static const int cellCount;
    static const float easyMode;
    static const float hardMode;
};

class Button{
private:
    char* text;
    float fontSize;
    Rectangle box;
public:
    Button(char*, float, Rectangle);
    void Draw();
    int IsPressed(Vector2 mousePos, int mousePressed);
};

Food::Food(){
    position = Vector2{0, 0};
    GenerateRandomImage();
}

Food::~Food(){
    UnloadTexture(texture);
}

void Food::Draw(){
    float x = position.x;
    float y = position.y;
    DrawTexture(texture, x * cellSize, y * cellSize, WHITE);
}

void Food::GenerateRandomPos(int start, int end){
    position.x = GetRandomValue(start, end);
    position.y = GetRandomValue(start, end);
}

Vector2 Food::GetPosition(){
    return position;
}

void Food::GenerateRandomImage(){
    int type = GetRandomValue(1, 4);

    switch(type){
    case 1:
        image = LoadImage("Graphic/Food/p_banana.png");
        break;
    case 2:
        image = LoadImage("Graphic/Food/p_cherry.png");
        break;
    case 3:
        image = LoadImage("Graphic/Food/p_lemon.png");
        break;
    case 4:
        image = LoadImage("Graphic/Food/p_watermelon.png");
        break;
    }

    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Snake::Snake(){
    body = {Vector2{8, 7}, Vector2{7, 7}, Vector2{6, 7}};
    direction = Vector2{1, 0};
}

void Snake::Draw(){
    for(unsigned int i = 0; i < body.size(); i++){
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment{x * cellSize, y * cellSize, float(cellSize), float(cellSize)};
        DrawRectangleRounded(segment, 0.5, 5, darkGreen);
    }
}

void Snake::Move(const Vector2 &direction){
    body.pop_back();

    int x = body[0].x - Game::offsetCellCount;
    int y = body[0].y - Game::offsetCellCount;

    x = x + direction.x;
    y = y + direction.y;

    x = (x + Game::cellCount) % Game::cellCount;
    y = (y + Game::cellCount) % Game::cellCount;

    x += Game::offsetCellCount;
    y += Game::offsetCellCount;

    body.push_front(Vector2{(float) x, (float) y});
    this->direction = direction;
}

deque<Vector2> Snake::GetBody(){
    return body;
}

void Snake::Grow(){
    Vector2 growPart = body.back();
    body.push_back(growPart);
}

Vector2 Snake::GetDirection(){
    return direction;
}

void Snake::BackHome(){
    body = {Vector2{8, 7}, Vector2{7, 7}, Vector2{6, 7}};
    direction = Vector2{1, 0};
}

const int Game::offsetCellCount = 3;
const int Game::cellCount = 30;
const Rectangle Game::lineBox{(float) offsetCellCount * cellSize, (float) offsetCellCount * cellSize, (float) cellCount * cellSize, (float) cellCount * cellSize};
const float Game::easyMode = 0.18;
const float Game::hardMode = 0.1;
int Game::highestScore = 0;

Game::Game(){
    Pause();
    score = 0;
    mode = easyMode;
    food = new Food; food->GenerateRandomPos(offsetCellCount, offsetCellCount + cellCount - 1);
    snake = new Snake;
    inputDirection = snake->GetDirection();
}

void Game::Update(){
    if(running == 0)
        return;

    snake->Move(inputDirection);
    if(IsSnakeCollided()){
        GameOver();
        GameReset();
    }

    Vector2 head = snake->GetBody().front();
    if(Vector2Equals(head, food->GetPosition())){
        score++;
        food->GenerateRandomImage();
        food->GenerateRandomPos(offsetCellCount, offsetCellCount + cellCount - 1);
        snake->Grow();
    }

    while(IsFoodCollided())
        food->GenerateRandomPos(offsetCellCount, offsetCellCount + cellCount - 1);
}

int Game::IsFoodCollided(){
    deque<Vector2> body = snake->GetBody();
    for(unsigned int i = 0; i < body.size(); i++)
        if(Vector2Equals(body[i], food->GetPosition()))
            return 1;
    return 0;
}

int Game::IsSnakeCollided(){
    deque<Vector2> body = snake->GetBody();
    for(unsigned int i = 1; i < body.size(); i++)
        if(Vector2Equals(body.front(), body[i]))
            return 1;
    return 0;
}

void Game::Draw(){
    snake->Draw();
    food->Draw();
}

void Game::GameOver(){
    Pause();
    highestScore = max(highestScore, score);

    Rectangle restartBox{(float) windowCellCount * cellSize * 2 / 5, windowCellCount * cellSize * (float) 2.3 / 5, (float) windowCellCount * cellSize / 5, (float) windowCellCount * cellSize / 18};
    Button restart(strdup("Restart"), 25, restartBox);

    int mousePressed = 0;
    Vector2 mousePos;

    while(WindowShouldClose() == false){
        mousePressed = 0;

        BeginDrawing();

        ClearBackground(green);
        DrawText(strdup("Game Over"), windowCellCount * cellSize * 1.25 / 5, windowCellCount * cellSize * 1.5 / 5, windowCellCount * cellSize / 10, darkGreen);
        DrawText(TextFormat("Score: %i", score), (float) windowCellCount * cellSize * 2 / 5, (float) windowCellCount * cellSize * 2 / 5, 40, darkGreen);
        restart.Draw();

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            mousePressed = 1;
            mousePos = GetMousePosition();

            if(restart.IsPressed(mousePos, mousePressed)){
                EndDrawing();
                break;
            }
        }

        EndDrawing();
    }
}

void Game::GameReset(){
    score = 0;
    food->GenerateRandomImage();
    food->GenerateRandomPos(offsetCellCount, offsetCellCount + cellCount - 1);
    snake->BackHome();
    inputDirection = snake->GetDirection();
}

void Game::StartGame(){
    running = 1;
}

void Game::Pause(){
    running = 0;
}

void Game::InputDirection(){
    if(IsKeyPressed(KEY_UP) && snake->GetDirection().y != 1){
        inputDirection = Vector2{0, -1};
        StartGame();
    }
    else if(IsKeyPressed(KEY_DOWN) && snake->GetDirection().y != -1){
        inputDirection = Vector2{0, 1};
        StartGame();
    }
    else if(IsKeyPressed(KEY_LEFT) && snake->GetDirection().x != 1){
        inputDirection = Vector2{-1, 0};
        StartGame();
    }
    else if(IsKeyPressed(KEY_RIGHT) && snake->GetDirection().x != -1){
        inputDirection = Vector2{1, 0};
        StartGame();
    }
}

int Game::GetScore(){
    return score;
}

int Game::GetHighestScore(){
    return highestScore;
}

void Game::changeMode(float mode){
    this->mode = mode;
}

int Game::EventTriggered(){
    double currentTime = GetTime();

    if(currentTime - lastUpdateTime >= mode){
        lastUpdateTime = currentTime;
        return 1;
    }

    return 0;
}

int Game::IsRunning(){
    return running;
}

Game::~Game(){
    delete food;
    delete snake;
}

Button::Button(char* text, float fontSize, Rectangle box){
    this->text = text;
    this->fontSize = fontSize;
    this->box = box;
}

void Button::Draw(){
    float x = box.x + box.width / 4;
    float y = box.y + box.height / 4;

    DrawRectangleLinesEx(box, 5, darkGreen);
    DrawText(text, x, y, fontSize, darkGreen);
}

int Button::IsPressed(Vector2 mousePos, int mousePressed){
    if(CheckCollisionPointRec(mousePos, box) && mousePressed)
        return 1;
    return 0;
}

float chooseMode(){
    Rectangle boxEasy{(float) windowCellCount * cellSize * 3 / 7, (float) windowCellCount * cellSize * 2 / 5, (float) windowCellCount * cellSize / 7, (float) windowCellCount * cellSize / 15};
    Button easy(strdup("Easy"), boxEasy.width * boxEasy.height / 280, boxEasy);

    Rectangle boxHard{(float) windowCellCount * cellSize * 3 / 7, (float) windowCellCount * cellSize * 68 / 140, (float) windowCellCount * cellSize / 7, (float) windowCellCount * cellSize / 15};
    Button hard(strdup("Hard"), boxHard.width * boxHard.height / 280, boxHard);

    int mousePressed = 0;
    Vector2 mousePos;

    while(WindowShouldClose() == false){
        mousePressed = 0;

        BeginDrawing();

        ClearBackground(green);
        DrawText(strdup("Choose Mode"), windowCellCount * cellSize * 0.8 / 5, windowCellCount * cellSize * 1.5 / 5, windowCellCount * cellSize / 10, darkGreen);
        easy.Draw();
        hard.Draw();

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            mousePressed = 1;
            mousePos = GetMousePosition();

            if(easy.IsPressed(mousePos, mousePressed)){
                EndDrawing();
                return Game::easyMode;
            }
            if(hard.IsPressed(mousePos, mousePressed)){
                EndDrawing();
                return Game::hardMode;
            }
        }

        EndDrawing();
    }

    return Game::easyMode;
}

void Menu(){
    Rectangle boxStart{(float) windowCellCount * cellSize * 2 / 5, (float) windowCellCount * cellSize * 2 / 5, (float) windowCellCount * cellSize / 5, (float) windowCellCount * cellSize / 15};
    Button start(strdup("Start"), boxStart.width * boxStart.height / 320, boxStart);

    Rectangle boxMode{(float) windowCellCount * cellSize * 3 / 7, (float) windowCellCount * cellSize * 68 / 140, (float) windowCellCount * cellSize / 7, (float) windowCellCount * cellSize / 15};
    Button b_mode(strdup("Mode"), boxMode.width * boxMode.height * 1.05 / 320, boxMode);

    int mousePressed = 0;
    Vector2 mousePosition;

    while(WindowShouldClose() == false){
        mousePressed = 0;

        BeginDrawing();

        ClearBackground(green);
        DrawText("SNAKE GAME", windowCellCount * cellSize * 0.8 / 5, windowCellCount * cellSize * 1.5 / 5, windowCellCount * cellSize / 10, darkGreen);
        start.Draw();
        b_mode.Draw();

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            mousePressed = 1;
            mousePosition = GetMousePosition();

            if(start.IsPressed(mousePosition, mousePressed)){
                EndDrawing();
                break;
            }
            if(b_mode.IsPressed(mousePosition, mousePressed)){
                EndDrawing();
                chooseMode();
            }
        }

        EndDrawing();
    }
}

void Play(){
    Game game;

    int mousePressed = 0;
    Vector2 mousePosition;

    Rectangle boxBack{(windowCellCount - Game::offsetCellCount - (float) windowCellCount / 7) * cellSize, (Game::offsetCellCount - (float) windowCellCount / 18) * cellSize, (float) windowCellCount * cellSize / 7, (float) windowCellCount * cellSize / 18};
    Button Back(strdup("Back"), 25, boxBack);

    Rectangle boxPause{(windowCellCount - Game::offsetCellCount - (float) windowCellCount * 2 / 7) * cellSize, (Game::offsetCellCount - (float) windowCellCount / 18) * cellSize, (float) windowCellCount * cellSize / 7, (float) windowCellCount * cellSize / 18};
    Button b_Pause(strdup("Pause"), 25, boxPause);

    Button b_play(strdup("Play"), 25, boxPause);

    while(WindowShouldClose() == false)
    {
        mousePressed = 0;

        BeginDrawing();

        ClearBackground(green);

        //display
        DrawText("Snake Game", Game::offsetCellCount * cellSize, 25, 50, darkGreen);

        if(game.IsRunning())
            b_Pause.Draw();
        else
            b_play.Draw();

        Back.Draw();
        DrawText(TextFormat("SCORE: %i", game.GetScore()), Game::offsetCellCount * cellSize, cellSize * (windowCellCount - Game::offsetCellCount) + 5, 40, darkGreen);
        DrawRectangleLinesEx(Game::lineBox, 5, darkGreen);
        game.Draw();

        //function of buttons on background
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            mousePressed = 1;
            mousePosition = GetMousePosition();

            if(Back.IsPressed(mousePosition, mousePressed)){
                EndDrawing();
                return;
            }

            int gameStatus = game.IsRunning();
            if(b_Pause.IsPressed(mousePosition, mousePressed) && gameStatus){
                game.Pause();
            }
            else if(b_play.IsPressed(mousePosition, mousePressed) && !gameStatus)
                game.StartGame();
        }

        //snake movement
        game.InputDirection();

        if(game.EventTriggered()){
            game.Update();
        }

        EndDrawing();
    };
}

int main()
{
    cout << "Starting the game...";

    InitWindow(cellSize * windowCellCount, cellSize * windowCellCount , "Snake Game");
    SetTargetFPS(60);

    while(WindowShouldClose() == false){
        Menu();
        Play();
    }

    CloseWindow();
    return 0;
}
