#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#include "Arduino.h"
#endif /*_ARDUINO_H_*/

#ifndef _BALL_H_
#define _BALL_H_

#include <string>

using namespace std;

class Ball
{
public:
	Ball();
	Ball(int, int);
	
	int getRow();
	int getCol();
	void shiftRowUp();
	void shiftRowDown();
	void shiftColLeft();
	void shiftColRight();

private:
	int row;
	int col;
	string rowDir;
	string colDir;
};

#endif /*_BALL_H_*/
