#include "block.h"

Block::Block(): currentSize(0)
{
}

int Block::isBlockThere(int row, int col)
{
	int exist = 0;
	for(int i = 0; i < currentSize; ++i)
	{
		if(blockRow[i] == row && blockCol[i] == col)
		{
			index = 1;
			break;
		}
	}
	return exist;
}

int Block::findBlockIndex(int row, int col)
{
	int index = 0;
	for(int i = 0; i < currentSize; ++i)
	{
		if(blockRow[i] == row && blockCol[i] == col)
		{
			index = i;
			break;
		}
	}
	return index;
}

void Block::setBlock(int row, int col)
{
	blockRow[currentSize] = row;
	blockCol[currentSize] = col;
	++currentSize;
}

void Block::removeBlock(int row, int col)
{
	int index = findBLockIndex(row, col);
	blockRow[index] = 0;
	blockCol[index] = 0;
	--currentSize;
}
