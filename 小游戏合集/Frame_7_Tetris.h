#pragma once
#include "Frame.h"

// 形状是由4*4的小格子组成的
#define SHAPE_BLOCK_NUM 4

int shapePrototype[][4][4] = {
	{
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}
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

class Shape {
private:
	Block* mBlocks[SHAPE_BLOCK_NUM][SHAPE_BLOCK_NUM];
	// 背板左上角的横纵坐标
	int mBoardX;
	int mBoardY;
	// 背景里横向和纵向的格子数
	int mXBlocks;
	int mYBlocks;
	// 格子的大小
	int mBlockSize;
	// 背景色
	COLORREF mColor;
	COLORREF mBkColor;
	
	int mShapeId;

public:
	Shape(int boardX, int boardY, int xBlocks, int yBlocks, int blockSize, COLORREF color, COLORREF bkColor, int shapeId) {
		this->mBoardX = boardX;
		this->mBoardY = boardY;
		this->mXBlocks = xBlocks;
		this->mYBlocks = yBlocks;
		this->mBlockSize = blockSize;
		this->mColor = color;
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

	void moveLeft() {
		if (canGoLeft() == false) {
			return;
		}
		erase();
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
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				Block* currBlock = mBlocks[i][j];
				if (currBlock != nullptr) {
					currBlock->moveDown();
				}
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
		for (int i = 0; i < SHAPE_BLOCK_NUM; i++) {
			for (int j = 0; j < SHAPE_BLOCK_NUM; j++) {
				if (shapePrototype[mShapeId][i][j] == 0) {
					mBlocks[i][j] = nullptr;
				} else {
					mBlocks[i][j] = new Block(mBoardX, mBoardY, mXBlocks, mYBlocks, mBlockSize, 5 + j, 5 + i, mBkColor, GREEN);
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
	int mXBlocks = 10;
	int mYBlocks = 20;

public:
	Frame_7_Tetris(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		mBoardBkColor = frameBkColor;
		mBoardOutlineColor = BLACK;

		testShape = new Shape(mBoardX, mBoardY, mXBlocks, mYBlocks, mBlockSize, RED, frameBkColor, 0);
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
		}

		FlushBatchDraw();
		EndBatchDraw();
	}
};


