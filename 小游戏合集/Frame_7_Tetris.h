#pragma once
#include "Frame.h"

class Block {
private:
	// 背板左上角的横纵坐标
	int mBoardX;
	int mBoardY;
	// 横向和纵向的格子数
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

	void erase() {
		int offset = 1;
		setfillcolor(mBkColor);
		solidrectangle(mBoardX + mX * mBlockSize + offset, mBoardY + mY * mBlockSize + offset,
			mBoardX + (mX + 1) * mBlockSize - offset, mBoardY + (mY + 1) * mBlockSize - offset);
	}

	void draw() {
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
		if (canGoLeft() == false) {
			return;
		}
		erase();
		mX--;
		draw();
	}

	void moveRight() {
		if (canGoRight() == false) {
			return;
		}
		erase();
		mX++;
		draw();
	}

	void moveDown() {
		if (canGoDown() == false) {
			return;
		}
		erase();
		mY++;
		draw();
	}
};

class Shape {
private:
	Block* mBlocks[4][4];
	// 背板左上角的横纵坐标
	int mBoardX;
	int mBoardY;
	// 横向和纵向的格子数
	int mXBlocks;
	int mYBlocks;
	// 当前格子处于背板的位置，不是坐标，相当于数组下标
	int mX;
	int mY;
	// 背景色
	COLORREF mBkColor;
	
	int mShapeId;

public:

};

class Frame_7_Tetris : public Frame {
private:
	int mBoardX = 330;
	int mBoardY = 130;
	int mBoardWidth;
	int mBoardHeight;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;

	Block* testBlock;

	// 每个格子的大小
	int mBlockSize = 32;
	// 横向和纵向的格子数
	int mXBlocks = 10;
	int mYBlocks = 20;

public:
	Frame_7_Tetris(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		testBlock = new Block(mBoardX, mBoardY, mXBlocks, mYBlocks, mBlockSize, 5, 5, frameBkColor, GREEN);
		testBlock->draw();
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
		setlinecolor(BLACK);
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
		setlinecolor(BLACK);
		for (int i = 1; i < 20; i++) {
			line(leftUpX, leftUpY + i * mBlockSize, rightUpX, rightUpY + i * mBlockSize);
		}
		for (int i = 1; i < 10; i++) {
			line(leftUpX + i * mBlockSize, leftUpY, leftDownX + i * mBlockSize, leftDownY);
		}

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
		if (vkcode == VK_LEFT) {
			testBlock->moveLeft();
		} else if (vkcode == VK_RIGHT) {
			testBlock->moveRight();
		} else if (vkcode == VK_DOWN) {
			testBlock->moveDown();
		}
	}
};


