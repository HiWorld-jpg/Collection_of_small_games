#pragma once
#include "Frame.h"

#define BOARD_SIZE_X 10
#define BOARD_SIZE_Y 20

// 形状是由4*4的小格子组成的
#define SHAPE_BLOCK_NUM 4

typedef struct ShapePrototype {
	int shapeBlock[4][4];
	COLORREF shapeColor;
	bool canRotate;
} ShapePrototype;

ShapePrototype shapePrototype[] = {
	{
		{
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		RED,
		true
	},
	{
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		RED,
		true
	},
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},
		BLUE,
		true
	},
	{
		{
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		BLUE,
		true
	},
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},
		GREEN,
		false
	},
	{
		{
			{0, 0, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		BROWN,
		false
	},
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},
		YELLOW,
		true
	},
	{
		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		CYAN,
		true
	}
};

class Block {
private:
	// 背板左上角的横纵坐标
	int mBoardX;
	int mBoardY;
	// 背景里横向和纵向的格子数
	int mXBlocks;
	int mYBlocks;
	// 格子的大小
	int mBlockSize;
	// 当前格子处于背板的位置，不是坐标，相当于数组下标
	int mX;
	int mY;
	// 背景色
	COLORREF mBkColor;
	// 前景色
	COLORREF mColor;

public:
	Block(int boardX, int boardY, int xBlocks, int yBlocks, int blockSize, int x, int y, COLORREF bkColor, COLORREF color) {
		setBoardXY(boardX, boardY);
		setXYBlocks(xBlocks, yBlocks);
		setBlockSize(blockSize);
		setXYPos(x, y);
		setBlockBkColor(bkColor);
		setBlockColor(color);
	}

	void setBoardXY(int boardX, int boardY) {
		mBoardX = boardX;
		mBoardY = boardY;
	}

	void setXYBlocks(int xBlocks, int yBlocks) {
		mXBlocks = xBlocks;
		mYBlocks = yBlocks;
	}

	void setBlockSize(int blockSize) {
		mBlockSize = blockSize;
	}

	void setXYPos(int x, int y) {
		setXPos(x);
		setYPos(y);
	}

	void setXPos(int x) {
		mX = x;
	}

	void setYPos(int y) {
		mY = y;
	}

	void setBlockBkColor(COLORREF bkColor) {
		mBkColor = bkColor;
	}

	void setBlockColor(COLORREF color) {
		mColor = color;
	}

	int getXPos() const {
		return mX;
	}

	int getYPos() const {
		return mY;
	}

	void erase() const {
		int offset = 1;
		setfillcolor(mBkColor);
		solidrectangle(mBoardX + mX * mBlockSize + offset, mBoardY + mY * mBlockSize + offset,
			mBoardX + (mX + 1) * mBlockSize - offset, mBoardY + (mY + 1) * mBlockSize - offset);
	}

	void draw() const {
		int offset = 1;
		setfillcolor(mColor);
		solidrectangle(mBoardX + mX * mBlockSize + offset, mBoardY + mY * mBlockSize + offset,
			mBoardX + (mX + 1) * mBlockSize - offset, mBoardY + (mY + 1) * mBlockSize - offset);
	}

	bool canGoLeft() const {
		if (mX == 0) {
			return false;
		}
		return true;
	}

	bool canGoRight() const {
		if (mX == mXBlocks - 1) {
			return false;
		}
		return true;
	}

	bool canGoDown() const {
		if (mY == mYBlocks - 1) {
			return false;
		}
		return true;
	}

	void moveLeft() {
		/*
		if (canGoLeft() == false) {
			return;
		}
		erase();
		*/
		mX--;
		draw();
	}

	void moveRight() {
		/*
		if (canGoRight() == false) {
			return;
		}
		erase();
		*/
		mX++;
		draw();
	}

	void moveDown() {
		/*
		if (canGoDown() == false) {
			return;
		}
		erase();
		*/
		mY++;
		draw();
	}
};

// 不new Blocks，负责管理以及delete Blocks
class Board {
private:
	Block* mBlocks[BOARD_SIZE_Y][BOARD_SIZE_X];
};

// new Block
class Shape {
private:
	Block* mBlocks[SHAPE_BLOCK_NUM][SHAPE_BLOCK_NUM];
	// 背板左上角的横纵坐标
	int mBoardX;
	int mBoardY;
	// 当前4*4形状左上角的横纵坐标
	int mX = 5;
	int mY = 5;
	// 背景里横向和纵向的格子数
	int mXBlocks;
	int mYBlocks;
	// 格子的大小
	int mBlockSize;
	// 背景色
	COLORREF mColor;
	COLORREF mBkColor;
	
	int mShapeId;
	bool mCanRotate;

public:
	Shape(int boardX, int boardY, int xBlocks, int yBlocks, int blockSize, COLORREF bkColor, int shapeId) {
		this->mBoardX = boardX;
		this->mBoardY = boardY;
		this->mXBlocks = xBlocks;
		this->mYBlocks = yBlocks;
		this->mBlockSize = blockSize;
		this->mBkColor = bkColor;
		this->mShapeId = shapeId;
		initWithShapePrototype();
	}

	bool canGoLeft() const {
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr && currBlock->canGoLeft() == false) {
					return false;
				}
			}
		}
		return true;
	}

	bool canGoRight() const {
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr && currBlock->canGoRight() == false) {
					return false;
				}
			}
		}
		return true;
	}

	bool canGoDown() const {
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr && currBlock->canGoDown() == false) {
					return false;
				}
			}
		}
		return true;
	}

	bool canRotate() const {
		if (mCanRotate == false) {
			return false;
		}
		// 相对于4*4形状的旋转中心
		int xCenter = 2;
		int yCenter = 2;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					// 相对于4*4形状的横纵坐标
					int currBlockX = j;
					int currBlockY = i;
					// 旋转后的位置相对于4*4形状的横纵坐标
					int xAfterRotate = xCenter + yCenter - currBlockY - 1;
					int yAfterRotate = yCenter - xCenter + currBlockX;
					// 旋转后的位置相对于背板网格的横纵坐标
					int finalBlockX = mX + xAfterRotate;
					int finalBlockY = mY + yAfterRotate;
					if (finalBlockX < 0 || finalBlockX >= mXBlocks) {
						return false;
					}
					if (finalBlockY < 0 || finalBlockY >= mYBlocks) {
						return false;
					}
				}
			}
		}
		return true;

	}

	void moveLeft() {
		if (canGoLeft() == false) {
			return;
		}
		erase();
		mX--;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->moveLeft();
				}
			}
		}
	}

	void moveRight() {
		if (canGoRight() == false) {
			return;
		}
		erase();
		mX++;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->moveRight();
				}
			}
		}
	}

	void moveDown() {
		if (canGoDown() == false) {
			return;
		}
		erase();
		mY++;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->moveDown();
				}
			}
		}
	}

	void rotate() {
		if (canRotate() == false) {
			return;
		}
		erase();
		// 将待旋转的方块记录下来
		int needRotateBlocks[SHAPE_BLOCK_NUM][SHAPE_BLOCK_NUM] = { 0 };
		// 旋转后的4*4形状
		Block* tempBlocks[SHAPE_BLOCK_NUM][SHAPE_BLOCK_NUM];
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				tempBlocks[i][j] = nullptr;
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					needRotateBlocks[i][j] = 1;
				}
			}
		}
		
		// 相对于4*4形状的旋转中心
		int xCenter = 2;
		int yCenter = 2;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr && needRotateBlocks[i][j] == 1) {
					// 相对于4*4形状的横纵坐标
					int currBlockX = j;
					int currBlockY = i;
					// 旋转后的位置相对于4*4形状的横纵坐标
					int xAfterRotate = xCenter + yCenter - currBlockY - 1;
					int yAfterRotate = yCenter - xCenter + currBlockX;
					// 旋转后的位置相对于背板网格的横纵坐标
					int finalBlockX = mX + xAfterRotate;
					int finalBlockY = mY + yAfterRotate;
					if (finalBlockX < 0 || finalBlockX >= mXBlocks) {
						continue;
					}
					if (finalBlockY < 0 || finalBlockY >= mYBlocks) {
						continue;
					}
					currBlock->setXYPos(finalBlockX, finalBlockY);
					currBlock->draw();

					tempBlocks[yAfterRotate][xAfterRotate] = currBlock;
				}
			}
		}

		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				mBlocks[i][j] = tempBlocks[i][j];
			}
		}
	}

	void draw() const {
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->draw();
				}
			}
		}
	}

	void erase() const {
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->erase();
				}
			}
		}
	}


private:
	void initWithShapePrototype() {
		mCanRotate = shapePrototype[mShapeId].canRotate;
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				if (shapePrototype[mShapeId].shapeBlock[i][j] == 0) {
					mBlocks[i][j] = nullptr;
				} else {
					mBlocks[i][j] = new Block(mBoardX, mBoardY, mXBlocks, mYBlocks, mBlockSize, mX + j, mY + i, mBkColor, shapePrototype[mShapeId].shapeColor);
				}
			}
		}
	}


};


class Frame_7_Tetris : public Frame {
private:
	int mBoardX = 330;
	int mBoardY = 130;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;

	Shape* testShape;

	// 每个格子的大小
	int mBlockSize = 32;
	// 横向和纵向的格子数
	int mXBlocks = BOARD_SIZE_X;
	int mYBlocks = BOARD_SIZE_Y;

public:
	Frame_7_Tetris(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		mBoardBkColor = frameBkColor;
		mBoardOutlineColor = BLACK;

		testShape = new Shape(mBoardX, mBoardY, mXBlocks, mYBlocks, mBlockSize, frameBkColor, 6);
	}

	virtual void init() override {
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
		for (int i = 0; i < mTextNum; i++) {
			mTextList[i]->draw();
		}
		drawBoard();
	}

	void drawBoard() const {
		setlinestyle(PS_SOLID, 1);
		setlinecolor(mBoardOutlineColor);
		int leftUpX = mBoardX;
		int leftUpY = mBoardY;
		int rightUpX = mBoardX + mBlockSize * mXBlocks;
		int rightUpY = mBoardY;
		int leftDownX = mBoardX;
		int leftDownY = mBoardY + mBlockSize * mYBlocks;
		int rightDownX = mBoardX + mBlockSize * mXBlocks;
		int rightDownY = mBoardY + mBlockSize * mYBlocks;

		line(leftUpX, leftUpY, rightUpX, rightUpY);
		line(leftUpX, leftUpY, leftDownX, leftDownY);
		line(rightDownX, rightDownY, rightUpX, rightUpY);
		line(rightDownX, rightDownY, leftDownX, leftDownY);

		setlinestyle(PS_SOLID, 1);
		setlinecolor(mBoardOutlineColor);
		for (int i = 1; i < 20; i++) {
			line(leftUpX, leftUpY + i * mBlockSize, rightUpX, rightUpY + i * mBlockSize);
		}
		for (int i = 1; i < 10; i++) {
			line(leftUpX + i * mBlockSize, leftUpY, leftDownX + i * mBlockSize, leftDownY);
		}
		testShape->draw();
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		BeginBatchDraw();

		if (eventIndex == 44) {
			drawBoard();
		}
		FlushBatchDraw();
		EndBatchDraw();
	}

	virtual void processKeyBoardUp(unsigned char vkcode) override {
		BeginBatchDraw();

		if (vkcode == VK_LEFT) {
			testShape->moveLeft();
		} else if (vkcode == VK_RIGHT) {
			testShape->moveRight();
		} else if (vkcode == VK_DOWN) {
			testShape->moveDown();
		} else if (vkcode == VK_UP) {
			testShape->rotate();
		}

		FlushBatchDraw();
		EndBatchDraw();
	}
};


