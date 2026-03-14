
#define RAYLIB_MACOS 1
#include <raylib.h>

int main(void)
{
	const int SCREENWIDTH = 800;
	const int SCREENHEIGHT = 700;

	//initialize window
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "pong clone");
	SetTargetFPS(60);


	//position of the ball
	Vector2 ballPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2};

	//paddle struct
	typedef struct {
		int x, y;
		int width, height;
		int speed;
	} Paddle;

	typedef struct {
		float x,y;
		float radius;
		float speedX, speedY;
	} Ball;

	//paddles
	Paddle leftpaddle = {50, 200, 20, 100, 5};
	Paddle rightpaddle = {730, 200, 20, 100, 5};

	Ball ball = {SCREENWIDTH/2, SCREENHEIGHT/2, 15, 3, 3};

	while (!WindowShouldClose())
	{
		//ball movements
		ball.x += ball.speedX;
		ball.y += ball.speedY;

		//left paddle movements
		if(IsKeyDown(KEY_W) && leftpaddle.y > 20) leftpaddle.y -= leftpaddle.speed;
		if(IsKeyDown(KEY_S) && leftpaddle.y < 580) leftpaddle.y += leftpaddle.speed;


		//right paddle movements
		if(IsKeyDown(KEY_UP) && rightpaddle.y > 20) rightpaddle.y -= rightpaddle.speed;
		if(IsKeyDown(KEY_DOWN) && rightpaddle.y  < 580) rightpaddle.y += rightpaddle.speed;

		//ball collision screenborders (no collision with sides obviously)
		if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREENHEIGHT) 
    		ball.speedY *= -1;

    	//check collisions with paddles
    	if (CheckCollisionCircleRec((Vector2){ball.x, ball.y},ball.radius, 
    		(Rectangle){leftpaddle.x, leftpaddle.y, leftpaddle.width, leftpaddle.height}) ||
    		CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
    			(Rectangle){rightpaddle.x, rightpaddle.y, rightpaddle.width, rightpaddle.height}))
    	{
    		ball.speedX *= -1;
    	}


		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText("pong in C", 350, 100, 20, DARKGRAY);

			DrawCircle(ball.x, ball.y, ball.radius, RED);
			DrawRectangle(leftpaddle.x, leftpaddle.y, leftpaddle.width, leftpaddle.height, LIGHTGRAY);
			DrawRectangle(rightpaddle.x, rightpaddle.y, rightpaddle.width, rightpaddle.height, LIGHTGRAY);


		EndDrawing();	
	}

	CloseWindow();
	
	return 0;
}


