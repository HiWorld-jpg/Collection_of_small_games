#pragma once
#include "Frame.h"
#include "TimeCount.h"
#include <stdlib.h>
#include <time.h>

typedef struct PicType {
	int id;
	char *path;
} PicType;

class Frame_5_lianliankan : public Frame {
private:
	int mBoardX;
	int mBoardY;
	int mBoardWidth;
	int mBoardHeight;
	int mCurrActiveX = -1;
	int mCurrActiveY = -1;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;
	CountDown mCountDown;
	static const int mPicMaxNum = 100;
	PicType mPic[mPicMaxNum]; // 只给0至mPicTypeNum-1的下标元素赋值，因此取元素时需要严格使用mPicTypeNum约束
	int mPicTypeNum;
	static const int mBoardXMax = 10;
	static const int mBoardYMax = 8;
	// mBoardData元素的值遵循以下约定
	// 0 ~ mPicTypeNum - 1代表各个图案
	// -1代表空值
	// -2是临时值，用来表示寻路算法中的脚印
	int mBoardData[mBoardXMax][mBoardYMax];  // 大小固定为横向20个格子，纵向15个格子
	bool mBoardDataPatten[mBoardXMax][mBoardYMax];  // 限制格子能不能加元素，true表示会填充元素，false表示不填充即当作空白处理

public:
	Frame_5_lianliankan(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex,
		int boardX, int boardY, int boardWidth, int boardHeight, COLORREF boardBkColor, COLORREF boardOutlineColor) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex),
		mCountDown(100, 100, 200, 80, frameBkColor, BLACK, 20) {

		mBoardX = boardX;
		mBoardY = boardY;
		mBoardWidth = boardWidth;
		mBoardHeight = boardHeight;
		mBoardBkColor = boardBkColor;
		mBoardOutlineColor = boardOutlineColor;

		initPic();

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
	}

	void initPic() {
		mPic[0].path = (char*)"./picture/lianliankan_picture/axial_symmetry.png";
		mPic[1].path = (char*)"./picture/lianliankan_picture/black_ball.png";
		mPic[2].path = (char*)"./picture/lianliankan_picture/block_L.png";
		mPic[3].path = (char*)"./picture/lianliankan_picture/chicken.png";
		mPic[4].path = (char*)"./picture/lianliankan_picture/dice.png";
		mPic[5].path = (char*)"./picture/lianliankan_picture/emoticon_man_with_sunglasses.png";
		mPic[6].path = (char*)"./picture/lianliankan_picture/pumpkin.png";
		mPic[7].path = (char*)"./picture/lianliankan_picture/gesture_palm.png";
		mPic[8].path = (char*)"./picture/lianliankan_picture/octopus.png";
		mPic[9].path = (char*)"./picture/lianliankan_picture/spade.png";
		mPic[10].path = (char*)"./picture/lianliankan_picture/wall_clock.png";

		mPicTypeNum = 11;   //注意不要超过mPicMaxNum
		for (int i = 0; i < mPicTypeNum; i++) {
			mPic[i].id = i;
		}
	}

	void initBoardDataPatten() {
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				mBoardDataPatten[i][j] = true;
			}
		}
		// 后续实现读取文件来切换模式
	}

	void initBoardData() {
		srand(time(NULL));
		initBoardDataPatten();
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				mBoardData[i][j] = rand() % mPicTypeNum;
			}
		}
		
	}

	void drawBoardLine() const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		setlinecolor(mBoardOutlineColor);

		int boardGridWidth = mBoardWidth / mBoardXMax;
		int boardGridHeight = mBoardHeight / mBoardYMax;
		for (int i = 0; i <= mBoardYMax; i++) {
			line(mBoardX, mBoardY + i * boardGridHeight, mBoardX + mBoardWidth, mBoardY + i * boardGridHeight);
		}
		for (int i = 0; i <= mBoardXMax; i++) {
			line(mBoardX + i * boardGridWidth, mBoardY, mBoardX + i * boardGridWidth, mBoardY + mBoardHeight);
		}
	}

	void drawBoard() const {
		setfillcolor(mBoardBkColor);
		solidrectangle(mBoardX, mBoardY, mBoardX + mBoardWidth, mBoardY + mBoardHeight);
		int boardGridWidth = mBoardWidth / mBoardXMax;
		int boardGridHeight = mBoardHeight / mBoardYMax;
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				int currGridValue = mBoardData[i][j];
				if (currGridValue >= 0 && currGridValue < mPicTypeNum) {
					IMAGE currImg;
					loadimage(&currImg, mPic[currGridValue].path, boardGridWidth, boardGridHeight);
					putimage(mBoardX + i * boardGridWidth, mBoardY + j * boardGridHeight, &currImg);
				}
			}
		}
		drawBoardLine();
	}

	bool checkMouseInBoard(int mouseX, int mouseY) {
		if (mouseX <= mBoardX || mouseX >= mBoardX + mBoardWidth) {
			return false;
		}
		if (mouseY <= mBoardY || mouseY >= mBoardY + mBoardHeight) {
			return false;
		}
		return true;
	}

	void calcBoardClickXYIndex(int mouseX, int mouseY, int& xIndex, int& yIndex) {
		int boardGridWidth = mBoardWidth / mBoardXMax;
		int boardGridHeight = mBoardHeight / mBoardYMax;
		xIndex = (mouseX - mBoardX) / boardGridWidth;
		yIndex = (mouseY - mBoardY) / boardGridHeight;
	}

	void drawActiveBlock() const {
		drawBoard();
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
		setlinecolor(RED);
		int boardGridWidth = mBoardWidth / mBoardXMax;
		int boardGridHeight = mBoardHeight / mBoardYMax;
		rectangle(mBoardX + mCurrActiveX * boardGridWidth,
			      mBoardY + mCurrActiveY * boardGridHeight,
				  mBoardX + (mCurrActiveX + 1) * boardGridWidth, 
			      mBoardY + (mCurrActiveY + 1) * boardGridHeight);
	}

	void eliminateBlock(int startX, int startY) {
		std::vector<std::pair<int, int>> resultPath;
		std::vector<std::shared_ptr<std::vector<std::pair<int, int>>>> workList;
		auto startPos = std::make_shared<std::vector<std::pair<int, int>>>();
		startPos->push_back(std::make_pair(startX, startY));
		int startPosValue = mBoardData[startX][startY];
		workList.push_back(startPos);
		while (workList.size() > 0) {
			int currX = workList[0]->back().first;
			int currY = workList[0]->back().second;
			// 上
			if (currY > 0) {
				if (currX == mCurrActiveX && currY - 1 == mCurrActiveY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(mCurrActiveX, mCurrActiveY));
					break;
				}
				if (mBoardData[currX][currY - 1] == -1) {
					auto currPos = std::make_shared<std::vector<std::pair<int, int>>>();
					currPos->insert(currPos->end(), workList[0]->begin(), workList[0]->end());
					currPos->push_back(std::make_pair(currX, currY - 1));
					workList.push_back(currPos);
					mBoardData[currX][currY - 1] = -2;
				}
			}
			// 下
			if (currY < mBoardYMax - 1) {
				if (currX == mCurrActiveX && currY + 1 == mCurrActiveY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(mCurrActiveX, mCurrActiveY));
					break;
				}
				if (mBoardData[currX][currY + 1] == -1) {
					auto currPos = std::make_shared<std::vector<std::pair<int, int>>>();
					currPos->insert(currPos->end(), workList[0]->begin(), workList[0]->end());
					currPos->push_back(std::make_pair(currX, currY + 1));
					workList.push_back(currPos);
					mBoardData[currX][currY + 1] = -2;
				}
			}
			// 左
			if (currX > 0) {
				if (currX - 1 == mCurrActiveX && currY == mCurrActiveY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(mCurrActiveX, mCurrActiveY));
					break;
				}
				if (mBoardData[currX - 1][currY] == -1) {
					auto currPos = std::make_shared<std::vector<std::pair<int, int>>>();
					currPos->insert(currPos->end(), workList[0]->begin(), workList[0]->end());
					currPos->push_back(std::make_pair(currX - 1, currY));
					workList.push_back(currPos);
					mBoardData[currX - 1][currY] = -2;
				}
			}
			// 右
			if (currX < mBoardXMax - 1) {
				if (currX + 1 == mCurrActiveX && currY == mCurrActiveY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(mCurrActiveX, mCurrActiveY));
					break;
				}
				if (mBoardData[currX + 1][currY] == -1) {
					auto currPos = std::make_shared<std::vector<std::pair<int, int>>>();
					currPos->insert(currPos->end(), workList[0]->begin(), workList[0]->end());
					currPos->push_back(std::make_pair(currX + 1, currY));
					workList.push_back(currPos);
					mBoardData[currX + 1][currY] = -2;
				}
			}
			workList.erase(workList.begin());
		}

		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				if (mBoardData[i][j] == -2) {
					mBoardData[i][j] = -1;
				}
			}
		}

		if (resultPath.size() > 0) {
			mBoardData[startX][startY] = -1;
			mBoardData[mCurrActiveX][mCurrActiveY] = -1;
			int boardGridWidth = mBoardWidth / mBoardXMax;
			int boardGridHeight = mBoardHeight / mBoardYMax;
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
			setlinecolor(BLUE);
			int resultPathSize = resultPath.size();
			for (int i = 0; i < resultPathSize - 1; i++) {
				int startLineX = mBoardX + resultPath[i].first * boardGridWidth + boardGridWidth / 2;
				int startLineY = mBoardY + resultPath[i].second * boardGridHeight + boardGridHeight / 2;
				int endLineX = mBoardX + resultPath[i + 1].first * boardGridWidth + boardGridWidth / 2;
				int endLineY = mBoardY + resultPath[i + 1].second * boardGridHeight + boardGridHeight / 2;
				line(startLineX, startLineY, endLineX, endLineY);
			}
			Sleep(500);
		}

	}

	void processBoardClick(int mouseX, int mouseY) {
		if (checkMouseInBoard(mouseX, mouseY) == false) {
			return;
		}
		int xIndex = 0;
		int yIndex = 0;
		calcBoardClickXYIndex(mouseX, mouseY, xIndex, yIndex);
		// 初次点击面板，初始化
		if (mCurrActiveX != -1 && mCurrActiveY != -1) {
			if (mBoardData[mCurrActiveX][mCurrActiveY] != -1 &&
				mBoardData[mCurrActiveX][mCurrActiveY] == mBoardData[xIndex][yIndex]) {
				eliminateBlock(xIndex, yIndex);
			}
		}
		mCurrActiveX = xIndex;
		mCurrActiveY = yIndex;
		drawBoard();
		drawActiveBlock();
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		// BeginBatchDraw();
		if (eventIndex == -1) {
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 36) {
			// mCountDown.startCounting();
			initBoardData();
			drawBoard();
		} else if (eventIndex == 37) {

		}
		// FlushBatchDraw();
		// EndBatchDraw();
	}

	virtual void processSomethingInLoop() override {
		mCountDown.doCounting();
	}
};