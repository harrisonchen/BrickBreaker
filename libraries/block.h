#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#include "Arduino.h"
#endif /*_ARDUINO_H_*/

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <list>

using namespace std;

class Block
{
public:
	Block();

	int isBlockThere(int, int);
	
	void setBlock(int, int);
	void removeBlock(int, int);

private:
	list<int> blockRow;
	list<int> blockCol;
};

#endif /*_BLOCK_H_*/
