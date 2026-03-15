#include <raylib.h>

//screen dimensions
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 700;

//paddles struct
typedef struct {
	int x, y;
	int width, height;
	int speed;
} Paddle;

//ball struct
typedef struct {
	float x,y;
	float radius;
	float speedX, speedY;
} Ball;


void drawline(void);

void PaddleCollision(Paddle paddle, int);

//scores
int leftScore;
int rightScore;

//paddles
Paddle leftpaddle = {50, 200, 20, 100, 5};
Paddle rightpaddle = {730, 200, 20, 100, 5};

Ball ball = {SCREENWIDTH/2.0f, SCREENHEIGHT/2.0f, 15, 3, 3};


int main(void)
{

	//initialize window
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "pong");
	SetTargetFPS(60);

	
	//main game loop
	while (!WindowShouldClose())
	{

		//ball movements
		ball.x += ball.speedX;
		ball.y += ball.speedY;

		//left paddle movements
		if(IsKeyDown(KEY_W) && leftpaddle.y > 0) leftpaddle.y -= leftpaddle.speed;
		if(IsKeyDown(KEY_S) && (leftpaddle.y + leftpaddle.height < SCREENHEIGHT)) leftpaddle.y += leftpaddle.speed;


		//right paddle movements
		if(IsKeyDown(KEY_UP) && rightpaddle.y > 0) rightpaddle.y -= rightpaddle.speed;
		if(IsKeyDown(KEY_DOWN) && (rightpaddle.y +rightpaddle.height <SCREENHEIGHT)) rightpaddle.y += rightpaddle.speed;

		//ball collision screenborders (no collision with screensides obviously)
		if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREENHEIGHT) 
    		ball.speedY *= -1; //flips direction

    	//check collisions with paddles
    	PaddleCollision(leftpaddle, 1);
    	PaddleCollision(rightpaddle, -1);

    	//reset ball
    	if (ball.x - ball.radius < 0){
    		ball.x = SCREENWIDTH/2.0f;
    		ball.y = SCREENHEIGHT/2.0f;	
    		ball.speedX *= 1;
    		rightScore += 1;	
    	}

    	if(ball.x + ball.radius > SCREENWIDTH){
    		ball.x = SCREENWIDTH/2.0f;
    		ball.y = SCREENHEIGHT/2.0f;
    		ball.speedX *= 1;
    		leftScore += 1;	
    	}

    	//draw to screen
		BeginDrawing();

			ClearBackground(RAYWHITE);

			drawline();

			DrawText("pong in C", 350, 50, 20, DARKGRAY);
			DrawText(TextFormat("%d", leftScore), 150, 100, 20, DARKGRAY);
			DrawText(TextFormat("%d", rightScore), 650, 100, 20 ,DARKGRAY);

			DrawCircle(ball.x, ball.y, ball.radius, DARKBLUE );

			DrawRectangle(leftpaddle.x, leftpaddle.y, leftpaddle.width, leftpaddle.height, LIGHTGRAY);
			DrawRectangle(rightpaddle.x, rightpaddle.y, rightpaddle.width, rightpaddle.height, LIGHTGRAY); 


		EndDrawing();	
	}

	CloseWindow();
	
	return 0;
} 


void drawline(void){ 
	int startposX = SCREENWIDTH /2;
	int endposX = startposX;
	int startposY = 0;
	int endposY = 700;
	Color color = {200,200,200,200};
	DrawLine(startposX, startposY, endposX, endposY, color);

}	


void PaddleCollision(Paddle paddle, int direction){
	if(CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius,
    		(Rectangle){paddle.x, paddle.y, paddle.width, paddle.height}))
    	{	
    		ball.x += 5 * direction; //prevent clipping
    		ball.speedX *= -1;
    	}
}

