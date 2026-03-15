#include <raylib.h>

//screen dimensions
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 700;

//gamestates
typedef enum {
	STATE_MENU,
	STATE_PLAYING,
	STATE_WIN
} GameState;

GameState state = STATE_MENU;

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



//paddles
Paddle leftpaddle = {50, 200, 20, 100, 5};
Paddle rightpaddle = {730, 200, 20, 100, 5};

//ball
Ball ball = {SCREENWIDTH/2.0f, SCREENHEIGHT/2.0f, 12, 4, 4};

//scores
int leftScore;
int rightScore;
int winner = 0;


//fn prototypes
void drawline(void);
void PaddleCollision(Ball *ball, Paddle *paddle, int);
void MoveBall(void);
void MovePaddles(void);
void ResetBall(Ball *ball);
void DrawToScreen(void);
void ChooseWinner(void);
void DrawWinScreen(void);
static void ResetGame(void);


int main()
{
	//initialize window & sounds
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "pong");
	InitAudioDevice();

	//load sounds
	Sound losePoint = LoadSound("resources/lose_point.wav");
	Sound gainPoint = LoadSound("resources/point_up.wav");
	Sound startGame = LoadSound("resources/start_game.wav");

	//fps
	SetTargetFPS(60);

	//main game loop
	while (!WindowShouldClose()){

		BeginDrawing();
	    ClearBackground(BLACK);

	    if (state == STATE_MENU){
	        DrawText("pong in C", 320, 50, 40, WHITE);
	        DrawText("Press Space to play!", 300, 200, 20, WHITE);
	        
	        if(IsKeyPressed(KEY_SPACE)){  
	        	PlaySound(startGame);
	            state = STATE_PLAYING;
	        }
	    }

		else if (state == STATE_PLAYING){
			//ball movements
			MoveBall();

			//paddle movement
			MovePaddles();

			//ball collision screenborders (no collision with screensides obviously)
			if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREENHEIGHT){
		    	ball.speedY *= -1; //flips direction
			}

		    //check collisions with paddles
		    PaddleCollision(&ball, &leftpaddle, 1);
		    PaddleCollision(&ball, &rightpaddle, -1);
		    	
		    //reset ball
		    if (ball.x - ball.radius < 0){
		    	PlaySound(losePoint);
		    	ResetBall(&ball);
		    	rightScore += 1;	
		    }

		    if(ball.x + ball.radius > SCREENWIDTH){
		    	PlaySound(gainPoint);
		    	ResetBall(&ball);
		    	leftScore += 1;	
		    }
		    ChooseWinner();

			//draw to screen
			DrawToScreen();		
			}	

		else if(state == STATE_WIN){
			DrawWinScreen();
		}

		EndDrawing();

		}
	UnloadSound(losePoint);
	UnloadSound(gainPoint);
	UnloadSound(startGame);
	CloseAudioDevice();
	CloseWindow();
	
	return 0;
} 

//draw line in the middle
void drawline(void){ 
	int startposX = SCREENWIDTH /2;
	int endposX = startposX;
	int startposY = 0;
	int endposY = SCREENHEIGHT;
	Color color = {255,255,255,255};
	DrawLine(startposX, startposY, endposX, endposY, color);

}	

void MovePaddles(){
	//left paddle movements
	if(IsKeyDown(KEY_W) && leftpaddle.y > 0) leftpaddle.y -= leftpaddle.speed;
	if(IsKeyDown(KEY_S) && (leftpaddle.y + leftpaddle.height < SCREENHEIGHT)) leftpaddle.y += leftpaddle.speed;

	//right paddle movements
	if(IsKeyDown(KEY_UP) && rightpaddle.y > 0) rightpaddle.y -= rightpaddle.speed;
	if(IsKeyDown(KEY_DOWN) && (rightpaddle.y +rightpaddle.height <SCREENHEIGHT)) rightpaddle.y += rightpaddle.speed;
}

//check paddle collision
void PaddleCollision(Ball *ball, Paddle *paddle, int direction){
	int paddleCenter = paddle->y + paddle->height /2;
	int difference = ball->y - paddleCenter;

	if(CheckCollisionCircleRec((Vector2){ball->x, ball->y}, ball->radius,
    		(Rectangle){paddle->x, paddle->y, paddle->width, paddle->height})){
			
    		ball->x += 5 * direction; //prevent clipping
    		ball->speedX *= -1;
    		ball->speedY = difference * 0.1; //sends the ball out at different angles relative to where the ball hit the paddle	
    	}
}

//reset everything
static void ResetGame(void){
    leftScore = 0;
    rightScore = 0;
    ball.x = SCREENWIDTH/2.0f;
    ball.y = SCREENHEIGHT/2.0f;
    ball.speedX = 4;
    ball.speedY = 4;
    leftpaddle.y = 200;
    rightpaddle.y = 200;
}

//pick winner
void ChooseWinner(void){
	if (leftScore >= 10){
		winner = 1; // left player wins
		state = STATE_WIN;
	}

	else if (rightScore >= 10){
		winner = 2; //right player wins
		state = STATE_WIN;
	}
}

void ResetBall(Ball *ball){
    ball->x = SCREENWIDTH/2.0f;
    ball->y = SCREENHEIGHT/2.0f;
    ball->speedX *= -1;
}

void DrawToScreen(void){
	drawline();

	DrawText(TextFormat("%d", leftScore), 150, 100, 20, GRAY);
	DrawText(TextFormat("%d", rightScore), 650, 100, 20, GRAY);

	DrawCircle(ball.x, ball.y, ball.radius, RED );

	DrawRectangle(leftpaddle.x, leftpaddle.y, leftpaddle.width, leftpaddle.height, GRAY);
	DrawRectangle(rightpaddle.x, rightpaddle.y, rightpaddle.width, rightpaddle.height, GRAY); 


}

void DrawWinScreen(void){
	if (winner ==1){
	DrawText("LEFT PLAYER WINS!", 250, 300, 30, WHITE);
	}
		    
	else if (winner == 2){
	DrawText("RIGHT PLAYER WINS!", 250, 300, 30, WHITE);
	}
		    
	DrawText("Press Space to restart!", 150, 350, 20, GRAY);
	DrawText("Press Escape to quit", 320, 500, 15, GRAY);

	if (IsKeyPressed(KEY_SPACE)){
	ResetGame();
	state = STATE_PLAYING;
	}
}

void MoveBall(void){
	ball.x += ball.speedX;
	ball.y += ball.speedY;
}










