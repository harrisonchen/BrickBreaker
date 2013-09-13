#include "ball.h"

Ball::Ball():row(0x40), col(0x04)
{
}

Ball::Ball(int r, int c):row(r), col(c)
{
}

int Ball::getRow()
{
	return row;
}

int Ball::getCol()
{
	return col;
}

void Ball::shiftRowUp()
{
	row = row >> 1;
}

void Ball::shiftRowDown()
{
	row = row << 1;
}

void Ball::shiftColLeft()
{
	col = col >> 1;
}

void Ball::shiftColRight()
{
	col = col << 1;
}

int Ball::hitPaddle(int paddleRow, int paddleLeft, int paddleMid, int paddleRight)
{
	if( (paddleRow == row) && ((paddleLeft == col) || (paddleMid == col) || (paddleRight == col)) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
