#ifndef _ARDUINO_H_
#define _ARDUINO_H_
#include "Arduino.h"
#endif /*_ARDUINO_H_*/

#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block
{
public:
	Block();

	int isBlockThere(int, int);
	int findBlockIndex(int, int);
	
	void setBlock(int, int);
	void removeBlock(int, int);

private:
	const int maxSize = 32;
	int blockRow[maxSize];
	int blockCol[maxSize];
	int currentSize = 0;

};

#endif /*_BLOCK_H_*/
