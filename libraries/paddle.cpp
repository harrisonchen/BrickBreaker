#include "paddle.h"

Paddle::Paddle():row(0x80)//, col[1](0x02), col[2](0x04), col[3](0x08)
{
	col[1] = 0x02;
	col[2] = 0x04;
	col[3] = 0x08;
}

Paddle::Paddle(int r, int c1, int c2, int c3):row(r)//, col[1](c1), col[2](c2), col[3](c3)
{
	col[1] = c1;
	col[2] = c2;
	col[3] = c3;
}

int Paddle::getLeftPaddle()
{
	return col[1];
}

int Paddle::getMidPaddle()
{
	return col[2];
}

int Paddle::getRightPaddle()
{
	return col[3];
}

void Paddle::shiftPaddleLeft()
{
	if(col[1] != 0x01)
	{
		for(int i = 0; i < 3; ++i)
		{
			col[i] = col[i] >> 1;
		}
	}
}

void Paddle::shiftPaddleRight()
{
	if(col[3] != 0x80)
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
