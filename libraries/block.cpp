#include "block.h"

Block::Block()
{
}

int Block::isBlockThere(int row, int col)
{
	int exist = 0;
	list<int>::iterator itRow = blockRow.begin();
	list<int>::iterator itCol = blockCol.begin();

	for(; (itRow != blockRow.end()) && (itCol != blockCol.end()); ++itRow, ++itCol)
	{
		if( (*itRow == row) && (*itCol == col) )
		{
			exist = 1;
			break;
		}
	}
	return exist;
}

void Block::setBlock(int row, int col)
{
	blockRow.push_back(row);
	blockCol.push_back(col);
}

void Block::removeBlock(int row, int col)
{
	blockRow.remove(row);
	blockCol.remove(col);
}
