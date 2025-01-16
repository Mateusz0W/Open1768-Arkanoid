#ifndef ARCANOID_H
#define ARCANOID_H

#include "Drawing.h"

void resetGame(int *paddle_x,int *ball_x,int *ball_y,int bricks[5][10])
{
    *paddle_x = (LCD_MAX_X - PADDLE_WIDTH) / 2;
    *ball_x = LCD_MAX_X / 2;
    *ball_y = LCD_MAX_Y / 2;
    ball_dx = 2;
    ball_dy = -2;

    for (int i = 0; i < BRICK_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_PER_ROW; j++)
        {
            bricks[i][j] = 1; // 1 indicates brick is present
        }
    }
}
void movePaddle(int dx,int *paddle_x,int *paddle_y)
{
    clear_paddle(paddle_x,paddle_y);
	  *paddle_x+=dx;                           
    if (*paddle_x < 0) 
			*paddle_x = 0;
    if (*paddle_x > LCD_MAX_X - PADDLE_WIDTH) 
			*paddle_x = LCD_MAX_X - PADDLE_WIDTH;
	
    drawPaddle(paddle_x,paddle_y);
}

void updateBall(int *ball_x,int *ball_y,int *paddle_x,int *paddle_y,int bricks[5][10],int *win)
{
    clearBall(ball_x,ball_y);
    *ball_x += ball_dx;
    *ball_y += ball_dy;

    // Bounce off walls
    if (*ball_x <= 0 || *ball_x >= LCD_MAX_X - BALL_SIZE)
        ball_dx = -ball_dx;
    if (*ball_y <= 0)
        ball_dy = -ball_dy;

    // Bounce off paddle
    if (*ball_y >= *paddle_y - BALL_SIZE && *ball_y <= *paddle_y && *ball_x >= *paddle_x && *ball_x <= *paddle_x + PADDLE_WIDTH)
    {
        ball_dy = -ball_dy;
    }

    // Hit bricks
    int brick_row = *ball_y / BRICK_HEIGHT;
    int brick_col = *ball_x / BRICK_WIDTH;
    if (brick_row < BRICK_ROWS && brick_col < BRICKS_PER_ROW && bricks[brick_row][brick_col] > 0)
    {
        bricks[brick_row][brick_col] = 0;
        ball_dy = -ball_dy;
			  drawBricks(bricks);
    }

    // Game over condition
    if (*ball_y >= LCD_MAX_Y)
    {
        *win = 0;
    }

    drawBall(ball_x,ball_y);
}

void drawGame(int *ball_x,int *ball_y,int *paddle_x,int *paddle_y,int bricks[5][10])
{
    fillBackground(LCDBlack);
    drawPaddle(paddle_x,paddle_y);
    drawBall(ball_x,ball_y);
    drawBricks(bricks);
}
void playGame(int *ball_x,int *ball_y,int *paddle_x,int *paddle_y,int bricks[5][10],int *win)
{
  
    updateBall(ball_x,ball_y,paddle_x,paddle_y,bricks,win);

    // Check win condition
    int bricks_remaining = 0;
    for (int i = 0; i < BRICK_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_PER_ROW; j++)
        {
            if (bricks[i][j] > 0)
            {
                bricks_remaining++;
            }
        }
    }
    if (bricks_remaining == 0)
    {
        *win = 2;
    }

    LPC_TIM0->IR = 1; // Clear timer interrupt flag
}
void GameOver(){
	fillBackground(LCDRed);
	drawString(100, 150, "GAME OVER", 0,LCDRed);
	drawString(150, 100, "WCISNIJ PRZYSICK BTN1", 0,LCDRed);
}
void winScreen(){
	fillBackground(LCDGreen);
	drawString(100, 150, "VICTORY!", 0,LCDGreen);
	drawString(150, 100, "WCISNIJ PRZYSICK BTN1", 0,LCDGreen);
}

#endif
