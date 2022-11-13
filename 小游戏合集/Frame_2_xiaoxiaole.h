#pragma once
#include "Frame.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

#define MAX_BLOCK_NUM 50
#define MAX_COLOR_TYPE 7

class Frame_2_xiaoxiaole : public Frame {
private:
	int mBoardX;
	int mBoardY;
	int mBoardWidth;
	int mBoardHeight;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;
	int mBoardData[MAX_BLOCK_NUM][MAX_BLOCK_NUM]; // -1代表没有颜色，数组下标的值表示对应颜色
	int mBlockNum = 8;  // 规格设置为 blockNum * blockNum
	int mColorTypeNum = 4;
	COLORREF mColorTypes[MAX_COLOR_TYPE] = { RED, GREEN, BLUE, YELLOW, CYAN, BROWN, MAGENTA };

public:
	Frame_2_xiaoxiaole(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex,
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

	void initBoardData() {
		// 随机数种子是unsigned int型的，而time是秒级的，所以如果按钮一秒内被按下多次，这一秒内的随机数种子就会是一样的
		// 进而导致一秒内按下多次但是界面不刷新。暂无解决方法
		srand(time(NULL));
		for (int i = 0; i < mBlockNum; i++) {
			for (int j = 0; j < mBlockNum; j++) {
				mBoardData[i][j] = rand() % mColorTypeNum;
			}
		}
	}

	void initBoard() {
		initBoardData();
		setfillcolor(mBoardBkColor);
		solidrectangle(mBoardX, mBoardY, mBoardX + mBoardWidth, mBoardY + mBoardHeight);
		drawBoardBlock();
		drawBoardLine();
	}

	void drawBoardLine() const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		setlinecolor(mBoardOutlineColor);

		int blockHeight = mBoardHeight / mBlockNum;
		int blockWidth = mBoardWidth / mBlockNum;
		for (int i = 0; i <= mBlockNum; i++) {
			line(mBoardX, mBoardY + i * blockHeight, mBoardX + mBoardWidth, mBoardY + i * blockHeight);
		}
		for (int i = 0; i <= mBlockNum; i++) {
			line(mBoardX + i * blockWidth, mBoardY, mBoardX + i * blockWidth, mBoardY + mBoardHeight);
		}
	}

	void drawBoardBlock() const {
		int blockHeight = mBoardHeight / mBlockNum;
		int blockWidth = mBoardWidth / mBlockNum;
		for (int i = 0; i < mBlockNum; i++) {
			for (int j = 0; j < mBlockNum; j++) {
				int currColorType = mBoardData[i][j];
				if (currColorType == -1) {
					setfillcolor(mBoardBkColor);
				} else {
					setfillcolor(mColorTypes[currColorType]);
				}
				solidrectangle(mBoardX + i * blockWidth, mBoardY + j * blockHeight, mBoardX + (i + 1) * blockWidth, mBoardY + (j + 1) * blockHeight);
			}
		}
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == -1) {
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 8) {
			initBoard();
		}
	}

	void processBoardClick(int mouseX, int mouseY) {
		if (mouseX <= mBoardX || mouseX >= mBoardX + mBoardWidth) {
			return;
		}
		if (mouseY <= mBoardY || mouseY >= mBoardY + mBoardHeight) {
			return;
		}
		int blockHeight = mBoardHeight / mBlockNum;
		int blockWidth = mBoardWidth / mBlockNum;
		int xIndex = (mouseX - mBoardX) / blockWidth;
		int yIndex = (mouseY - mBoardY) / blockHeight;
		int currColorType = mBoardData[xIndex][yIndex];
		if (currColorType == -1) {
			return;
		}
		BeginBatchDraw();

		int clearCount = clearSameColor(xIndex, yIndex);
		letBlockDrop();
		drawBoardBlock();
		drawBoardLine();
		char temp[100] = { 0 };
		snprintf(temp, 100, "%d", clearCount);
		mTextList[2]->setText(temp);
		mTextList[2]->setFontColor(mColorTypes[currColorType]);
		mTextList[2]->draw();
		EndBatchDraw();
	}

	int clearSameColor(int targetXIndex, int targetYIndex) {
		int clearedBlockCount = 0;
		int currColorType = mBoardData[targetXIndex][targetYIndex];
		if (currColorType == -1) {
			return 0;
		}
		std::vector<std::pair<int, int>> workList;
		workList.push_back(std::make_pair(targetXIndex, targetYIndex));
		while (workList.size() != 0) {
			// 清除当前位置的颜色
			auto it = workList.begin();
			if (mBoardData[it->first][it->second] == currColorType) {
				mBoardData[it->first][it->second] = -1;
				clearedBlockCount++;
			}
			// 观察上方是否同色
			it = workList.begin();
			if (it->second > 0 && mBoardData[it->first][it->second - 1] == currColorType) {
				workList.push_back(std::make_pair(it->first, it->second - 1));
			}
			// 观察下方是否同色
			it = workList.begin();
			if (it->second < mBlockNum - 1 && mBoardData[it->first][it->second + 1] == currColorType) {
				workList.push_back(std::make_pair(it->first, it->second + 1));
			}
			// 观察左方是否同色
			it = workList.begin();
			if (it->first > 0 && mBoardData[it->first - 1][it->second] == currColorType) {
				workList.push_back(std::make_pair(it->first - 1, it->second));
			}
			// 观察右方是否同色
			it = workList.begin();
			if (it->first < mBlockNum - 1 && mBoardData[it->first + 1][it->second] == currColorType) {
				workList.push_back(std::make_pair(it->first + 1, it->second));
			}

			workList.erase(workList.begin());
		}
		return clearedBlockCount;
	}

	void letBlockDrop() {
		for (int i = 0; i < mBlockNum; i++) {
			for (int j = mBlockNum - 1; j >= 0; j--) {
				if (mBoardData[i][j] == -1) {
					continue;
				}
				int currY = j;
				while (currY < mBlockNum - 1) {
					if (mBoardData[i][currY + 1] == -1) {
						mBoardData[i][currY + 1] = mBoardData[i][currY];
						mBoardData[i][currY] = -1;
					} else{
						break;
					}
					currY++;
				}
			}
		}
	}
};