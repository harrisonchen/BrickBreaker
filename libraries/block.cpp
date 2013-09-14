#include "block.h"

Block::Block()
{
}

int Block::isBlockThere(int row, int col)
{
	if(blockRow.isInList(row) && blockCol.isInList(col))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Block::setBlock(int row, int col)
{
	blockRow.pushBack(row);
	blockCol.pushBack(col);
}

void Block::removeBlock(int row, int col)
{
	blockRow.deleteNode(row);
	blockCol.deleteNode(col);
}
