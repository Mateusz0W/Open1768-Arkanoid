#ifndef DRAWING_H
#define DRAWING_H

#include "lcd_lib\Open1768_LCD.h"
#include "lcd_lib\LCD_ILI9325.h"
#include "lcd_lib\asciiLib.h"
#include "tp_lib\TP_Open1768.h"

#include "Uart.h"

#define PADDLE_WIDTH 50
#define PADDLE_HEIGHT 10
#define BALL_SIZE 8
#define BRICK_WIDTH 30
#define BRICK_HEIGHT 10
#define BRICKS_PER_ROW 10
#define BRICK_ROWS 5

int ball_dx = 2;
int ball_dy = -2;


void LCD_setup()
{
	lcdConfiguration();
	init_ILI9325();
}

void check_reg()
{

	lcdReadReg(OSCIL_ON);
	if(lcdReadReg(OSCIL_ON) == 0x9325 || lcdReadReg(OSCIL_ON) == 0x9328)
	{
		send("ILI");
	}
	else send("SDD");
}
void drawPoint(int x, int y, int color)
{
	lcdSetCursor(LCD_MAX_X - x, LCD_MAX_Y - y);
	lcdWriteReg(DATA_RAM, color);
}
void drawChar(int x, int y, char c, int font,int background_color)
{
	unsigned char character[16];
	GetASCIICode(font, character, c);
	
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (((character[i] >> j) & 1) == 1)
				drawPoint(x + i, LCD_MAX_Y - 8 - y + j, LCDWhite);
			else
				drawPoint(x + i, LCD_MAX_Y - 8 - y + j,background_color);
		}
	}
}

void drawString(int x, int y, char* str, int font,int background_color)
{
	int i = 0;
	while(str[i] != '\0')
	{
		drawChar(x, y + i * 8, str[i], font,background_color);
		i++;
	}
}
void resetRam()
{
	lcdWriteReg(HADRPOS_RAM_START, 0);
	lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
	
	lcdWriteReg(VADRPOS_RAM_START, 0);
	lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
	
	lcdSetCursor(0, 0);
}

void drawRectangle(int pos_x, int pos_y, int size_x, int size_y, int color)
{
	lcdWriteReg(HADRPOS_RAM_START, pos_x);
	lcdWriteReg(HADRPOS_RAM_END, pos_x + size_x - 1);
	
	lcdWriteReg(VADRPOS_RAM_START, pos_y);
	lcdWriteReg(VADRPOS_RAM_END, pos_y + size_y - 1);
	
	lcdSetCursor(pos_x, pos_y);
	lcdWriteIndex(DATA_RAM);
	for (int i = 0; i < size_x * size_y; i++)
	{
			lcdWriteData(color);
	}
	
	resetRam();
}
void fillBackground(int color)
{
	lcdSetCursor(0, 0);
	lcdWriteIndex(DATA_RAM);
	for (int i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++)
			lcdWriteData(color);
}
void drawPaddle(int *paddle_x,int *paddle_y)
{
    drawRectangle(*paddle_x, *paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, LCDWhite);
}

void drawBall(int *ball_x,int *ball_y)
{
    drawRectangle(*ball_x, *ball_y, BALL_SIZE, BALL_SIZE, LCDRed);
}

void drawBricks(int bricks[5][10])
{
    for (int i = 0; i < BRICK_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_PER_ROW; j++)
        {
            if (bricks[i][j] > 0)
            {
				int color;
				if(i%2==0)
					color= j%2 == 0 ? LCDGreen :LCDMagenta;
				else
					color = j%2 == 0 ? LCDGinger : LCDPastelBlue;
                drawRectangle(j * BRICK_WIDTH, i * BRICK_HEIGHT, BRICK_WIDTH - 2, BRICK_HEIGHT - 2, color);
            }
			else
				drawRectangle(j * BRICK_WIDTH, i * BRICK_HEIGHT, BRICK_WIDTH - 2, BRICK_HEIGHT - 2, LCDBlack);
        }
    }
}

void clearBall(int *ball_x,int *ball_y)
{
    drawRectangle(*ball_x, *ball_y, BALL_SIZE, BALL_SIZE, LCDBlack);
}

void clear_paddle(int *paddle_x,int *paddle_y)
{
    drawRectangle(*paddle_x, *paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, LCDBlack);
}
#endif
