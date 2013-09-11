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
