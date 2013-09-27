#include "paddle.h"

Paddle::Paddle():row(0x80)
{
	col[0] = 0x02;
	col[1] = 0x04;
	col[2] = 0x08;
}

Paddle::Paddle(int r, int c1, int c2, int c3):row(r)
{
	col[0] = c1;
	col[1] = c2;
	col[2] = c3;
}

int Paddle::getPaddleRow()
{
	return row;
}

int Paddle::getLeftPaddle()
{
	return col[0];
}

int Paddle::getMidPaddle()
{
	return col[1];
}

int Paddle::getRightPaddle()
{
	return col[2];
}

void Paddle::shiftPaddleLeft()
{
	if(col[0] != 0x01)
	{
		for(int i = 0; i < 3; ++i)
		{
			col[i] = col[i] >> 1;
		}
	}
}

void Paddle::shiftPaddleRight()
{
	if(col[2] != 0x80)
	{
		for(int i = 0; i < 3; ++i)
		{
			col[i] = col[i] << 1;
		}
	}
}

int Paddle::hitBall(int ballRow, int ballCol)
{
	if( (ballRow == row) && ((ballCol == col[1]) || (ballCol == col[2]) || (ballCol == col[3])) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
