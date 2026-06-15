#include <iostream>
#include <string.h>
#include <raylib.h>

using namespace std;

const int screenWidth = 1280;
const int screenHeight = 800;
const char* screenTitle = "Ping Pong";

const int paddleHeight = 140;
const int paddleWidth = 20;

int paddle1XPos = 15;
int paddle2XPos = screenWidth - (paddle1XPos + paddleWidth);
int paddle1YPos = (screenHeight / 2) - (paddleHeight / 2);
int paddle2YPos = (screenHeight / 2) - (paddleHeight / 2);

int player1Score = 0;
int player2Score = 0;

class Ball
{
public:
    float x, y;
    int speedX, speedY;
    int radius;

    void Draw()
    {
        DrawCircle((int)x, (int)y, radius, WHITE);
    }

    void Update()
    {
        x += speedX;
        y += speedY;
        
        if(y + radius >= screenHeight || y - radius <= 0)
        {
            speedY *= -1;
        }

        if(x + radius >= screenWidth)
        {
            player1Score++;
            ResetBall();
        }

        if(x - radius <= 0)
        {
            player2Score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = screenWidth/2;
        y = screenHeight/2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
public:

    float x, y;
    float width, height;
    int speed;
    bool controls; //TRUE = UP, DOWN --- FALSE = W, S

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE); // left paddle
    }

    void Update()
    {
        if(IsKeyDown(KEY_DOWN) && controls)
            y += speed;

        if(IsKeyDown(KEY_UP) && controls)
            y -= speed;

        if(IsKeyDown(KEY_S) && !controls)
            y += speed;

        if(IsKeyDown(KEY_W) && !controls)
            y -= speed;

        if(y <= 0)
            y = 0;

        if(y + height >= screenHeight)
            y = screenHeight - height;
    }

};

Ball ball;
Paddle player1;
Paddle player2;

void DrawScreen()
{
    DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE); // middle line
    player1.Draw();
    player2.Draw();
    ball.Draw();
}


int main()
{
    InitWindow(screenWidth, screenHeight, screenTitle);
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedX = 7;
    ball.speedY = 7;

    player1.x = paddle1XPos;
    player1.y = paddle1YPos;
    player1.height = paddleHeight;
    player1.width = paddleWidth;
    player1.speed = 8;
    player1.controls = false;

    player2.x = paddle2XPos;
    player2.y = paddle2YPos;
    player2.height = paddleHeight;
    player2.width = paddleWidth;
    player2.speed = 8;
    player2.controls = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Update
        ball.Update();
        player1.Update();
        player2.Update();

        //Checking Ball and Paddle collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player1.x, player1.y, player1.width, player1.height}))
        {
            ball.speedX *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height}))
        {
            ball.speedX *= -1;
        }

        // Draw
        ClearBackground(BLACK);
        DrawScreen();

        DrawText(TextFormat("%i", player1Score), screenWidth/4 - 20, 20, 60, WHITE);
        DrawText(TextFormat("%i", player2Score), 3 * screenWidth/4 - 20, 20, 60, WHITE);

        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}