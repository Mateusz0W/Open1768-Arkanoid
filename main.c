#include "Joystick.h"
#include "Arcanoid.h"


int main(void){
	//Joystick
	int X;
	int Y;
	uint8_t data[5];
	//Drawing
	int paddle_x = (LCD_MAX_X - PADDLE_WIDTH) / 2;
	int paddle_y = LCD_MAX_Y - 20;
	int ball_x = LCD_MAX_X / 2;
	int ball_y = LCD_MAX_Y / 2;
	int bricks[BRICK_ROWS][BRICKS_PER_ROW];
	int win = -1;
	//Setup
	set_up_UART0();
	LCD_setup();
	timer0Init();
	SPIInit();
	
	while(1){
		 readJoystick(&X,&Y,data);
    if(X<450)
			movePaddle(-5,&paddle_x,&paddle_y);
		else if(X>500)
			movePaddle(5,&paddle_x,&paddle_y);
		
		if(win==2){
			winScreen();
			win=-2;
		}
		else if (win == 1)
		{
			playGame(&ball_x,&ball_y,&paddle_x,&paddle_y,bricks,&win);
			delayMicroseconds(10000);
		}
		else if (win == 0)
		{
			GameOver();
			win=-2;
		}
		else if (win == -1)
		{
				resetGame(&paddle_x,&ball_x,&ball_y,bricks);
				drawGame(&ball_x,&ball_y,&paddle_x,&paddle_y,bricks);
				win = 1;
		}
		else if(win==-2){
			if(data[4] ==2)
				win=-1;
		}
	}
	
}

