#pragma once
#include "Frame.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "FunctionUtils.h"
#include "FileDialog.h"

class BoardData {
private:
	int mValue[9][9];          // 0代表未填充数值，1-9表示填入的数
	int mValueCanBeWrite[9][9];  // 1代表可写，0表示不可写

public:
	BoardData() {};

	void initBoardData() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				mValue[i][j] = 0;
				mValueCanBeWrite[i][j] = 1;
			}
		}
	}

	int getValue(int xIndex, int yIndex) const {
		if (xIndex < 0 || xIndex > 8) {
			return -1;
		}
		if (yIndex < 0 || yIndex > 8) {
			return -1;
		}
		return mValue[xIndex][yIndex];
	}

	void readDataFromFile() {

	}
};

class Frame_3_shudu : public Frame {
private:
	int mBoardX;
	int mBoardY;
	int mBoardWidth;
	int mBoardHeight;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;
	BoardData mBoardData;

public:
	Frame_3_shudu(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex,
		int boardX, int boardY, int boardWidth, int boardHeight, COLORREF boardBkColor, COLORREF boardOutlineColor) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		mBoardX = boardX;
		mBoardY = boardY;
		mBoardWidth = boardWidth;
		mBoardHeight = boardHeight;
		mBoardBkColor = boardBkColor;
		mBoardOutlineColor = boardOutlineColor;
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

		initBoard();
	}

	void initBoard() {
		initBoardData();
		setfillcolor(mBoardBkColor);
		solidrectangle(mBoardX, mBoardY, mBoardX + mBoardWidth, mBoardY + mBoardHeight);
		drawBoardValue();
		drawBoardLine();
	}

	void initBoardData() {
		mBoardData.initBoardData();
	}

	void drawBoardLine() const {
		setlinecolor(mBoardOutlineColor);
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		for (int i = 0; i <= 9; i++) {
			if (i == 0 || i == 3 || i == 6 || i == 9) {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 4);
			} else {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
			}
			line(mBoardX, mBoardY + i * blockHeight, mBoardX + mBoardWidth, mBoardY + i * blockHeight);
		}
		for (int i = 0; i <= 9; i++) {
			if (i == 0 || i == 3 || i == 6 || i == 9) {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 4);
			} else {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
			}
			line(mBoardX + i * blockWidth, mBoardY, mBoardX + i * blockWidth, mBoardY + mBoardHeight);
		}
	}

	void drawBoardValue() const {
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		char numToStr[2] = { 0 };
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				int currBoardValue = mBoardData.getValue(i, j);
				if (currBoardValue < 1 || currBoardValue > 9) {
					continue;
				}
				int currX = mBoardX + i * blockWidth;
				int currY = mBoardY + j * blockHeight;
				snprintf(numToStr, 2, "%d", currBoardValue);
				numToStr[1] = '\0';
				FunctionUtils::printStrToRectangleArea(currX, currY, blockWidth, blockHeight, numToStr, BLACK);
			}
		}
		
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == -1) {
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 11) {

		} else if (eventIndex == 12) {

		}
	}

	void processBoardClick(int mouseX, int mouseY) {

	}

};