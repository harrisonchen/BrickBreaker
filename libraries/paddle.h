#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <string>

using namespace std;

class Paddle
{
public:
	Paddle();
	Paddle(int, int, int, int);

	int getLeftPaddle();
	int getMidPaddle();
	int getRightPaddle();

	void shiftPaddleLeft();
	void shiftPaddleRight();

	int hitBall(int, int);

private:
	int row;
	int col[3];
};

#endif /*_PADDLE_H_*/
