#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#include "Arduino.h"
#endif /*_ARDUINO_H_*/

#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <string>

using namespace std;

class Paddle
{
public:
	Paddle();
	Paddle(int, int, int, int);

	int getPaddleRow();
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
