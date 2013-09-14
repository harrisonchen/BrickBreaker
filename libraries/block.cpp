#include "block.h"

Block::Block()
{
}

int Block::isBlockThere(int row, int col)
{
	if(blocks.isInList(row, col))
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
	blocks.pushBack(row, col);
}

void Block::removeBlock(int row, int col)
{
  blocks.deleteNode(row, col);
}
