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
	// X:Y=5:4是合适比例
	static const int mBoardXMax = 10;
	static const int mBoardYMax = 8;
	// mBoardData元素的值遵循以下约定
	// 0 ~ mPicTypeNum - 1代表各个图案
	// -1代表空值
	// -2是临时值，用来表示寻路算法中的脚印
	int mBoardData[mBoardXMax][mBoardYMax];
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
		mPic[3].path = (char*)"./picture/lianliankan_picture/block_long.png";
		mPic[4].path = (char*)"./picture/lianliankan_picture/block_reverse_L.png";
		mPic[5].path = (char*)"./picture/lianliankan_picture/block_reverse_z.png";
		mPic[6].path = (char*)"./picture/lianliankan_picture/block_T.png";
		mPic[7].path = (char*)"./picture/lianliankan_picture/block_z.png";
		mPic[8].path = (char*)"./picture/lianliankan_picture/blue_ball.png";
		mPic[9].path = (char*)"./picture/lianliankan_picture/bomb.png";
		mPic[10].path = (char*)"./picture/lianliankan_picture/chicken.png";
		mPic[11].path = (char*)"./picture/lianliankan_picture/cyan_ball.png";
		mPic[12].path = (char*)"./picture/lianliankan_picture/desk_clock.png";
		mPic[13].path = (char*)"./picture/lianliankan_picture/dice.png";
		mPic[14].path = (char*)"./picture/lianliankan_picture/emoticon_man_with_crying_face.png";
		mPic[15].path = (char*)"./picture/lianliankan_picture/emoticon_man_with_red_face.png";
		mPic[16].path = (char*)"./picture/lianliankan_picture/emoticon_man_with_sunglasses.png";
		mPic[17].path = (char*)"./picture/lianliankan_picture/emoticon_man_with_sweat.png";
		mPic[18].path = (char*)"./picture/lianliankan_picture/gesture_fist.png";
		mPic[19].path = (char*)"./picture/lianliankan_picture/gesture_no.png";
		mPic[20].path = (char*)"./picture/lianliankan_picture/gesture_palm.png";
		mPic[21].path = (char*)"./picture/lianliankan_picture/gesture_yes.png";
		mPic[22].path = (char*)"./picture/lianliankan_picture/green_ball.png";
		mPic[23].path = (char*)"./picture/lianliankan_picture/loop.png";
		mPic[24].path = (char*)"./picture/lianliankan_picture/magnifier.png";
		mPic[25].path = (char*)"./picture/lianliankan_picture/octopus.png";
		mPic[26].path = (char*)"./picture/lianliankan_picture/phone.png";
		mPic[27].path = (char*)"./picture/lianliankan_picture/pumpkin.png";
		mPic[28].path = (char*)"./picture/lianliankan_picture/purple_ball.png";
		mPic[29].path = (char*)"./picture/lianliankan_picture/radar.png";
		mPic[30].path = (char*)"./picture/lianliankan_picture/real_diamond.png";
		mPic[31].path = (char*)"./picture/lianliankan_picture/road_block.png";
		mPic[32].path = (char*)"./picture/lianliankan_picture/shark.png";
		mPic[33].path = (char*)"./picture/lianliankan_picture/spade.png";
		mPic[34].path = (char*)"./picture/lianliankan_picture/star.png";
		mPic[35].path = (char*)"./picture/lianliankan_picture/wall_clock.png";
		mPic[36].path = (char*)"./picture/lianliankan_picture/xbox.png";
		mPic[37].path = (char*)"./picture/lianliankan_picture/yellow_ball.png";
		mPic[38].path = (char*)"./picture/lianliankan_picture/red_ball.png";

		mPicTypeNum = 39;   //注意不要超过mPicMaxNum
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
		std::vector<std::pair<int, int>> emptyBlocks;
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				if (mBoardDataPatten[i][j] == true) {
					emptyBlocks.push_back(std::make_pair(i, j));
				}
			}
		}
		// 保证两两配对，否则会出现有方块无法消除的情况
		if (emptyBlocks.size() % 2 != 0) {
			return;
		}
		while (emptyBlocks.size() > 0) {
			// 随机选取图片
			int currPicType = rand() % mPicTypeNum;
			// 随机决定要填充到哪个格子里
			int posIndex = rand() % emptyBlocks.size();
			int currX = emptyBlocks[posIndex].first;
			int currY = emptyBlocks[posIndex].second;
			mBoardData[currX][currY] = currPicType;

			emptyBlocks.erase(emptyBlocks.begin() + posIndex);

			// 再次随机选一个格子填充相同颜色
			posIndex = rand() % emptyBlocks.size();
			currX = emptyBlocks[posIndex].first;
			currY = emptyBlocks[posIndex].second;
			mBoardData[currX][currY] = currPicType;

			emptyBlocks.erase(emptyBlocks.begin() + posIndex);
		}
	}
	
	void shuffleBoard() {
		srand(time(NULL));
		std::vector<std::pair<int, int>> validBlocks;
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				if (mBoardDataPatten[i][j] == true) {
					validBlocks.push_back(std::make_pair(i, j));
				}
			}
		}
		// 保证两两配对，否则会出现有方块无法消除的情况
		if (validBlocks.size() % 2 != 0) {
			return;
		}
		// 图片类型id <索引X, 索引Y>
		std::vector<std::pair<int, std::pair<int, int>>> existedBlocks;
		for (int i = 0; i < mBoardXMax; i++) {
			for (int j = 0; j < mBoardYMax; j++) {
				if (mBoardData[i][j] >= 0) {
					existedBlocks.push_back(std::make_pair(mBoardData[i][j], std::make_pair(i, j)));
					mBoardData[i][j] = -1;
				}
			}
		}
		while (existedBlocks.size() > 0) {
			// 随机决定要填充到哪个格子里
			int posIndex = rand() % validBlocks.size();
			int destX = validBlocks[posIndex].first;
			int destY = validBlocks[posIndex].second;
			int srcX = existedBlocks[0].second.first;
			int srcY = existedBlocks[0].second.second;
			int srcPicType = existedBlocks[0].first;
			mBoardData[destX][destY] = srcPicType;

			validBlocks.erase(validBlocks.begin() + posIndex);
			existedBlocks.erase(existedBlocks.begin());
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

	void drawActiveBlock(int currX, int currY, COLORREF color) const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
		setlinecolor(color);
		int boardGridWidth = mBoardWidth / mBoardXMax;
		int boardGridHeight = mBoardHeight / mBoardYMax;
		rectangle(mBoardX + currX * boardGridWidth,
			      mBoardY + currY * boardGridHeight,
				  mBoardX + (currX + 1) * boardGridWidth,
			      mBoardY + (currY + 1) * boardGridHeight);
	}

	// 寻找(startX, startY)和(endX, endY)之间的通路
	void searchConnectedPath(int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>> &resultPath) {
		// 空格不做操作
		if (mBoardData[startX][startY] == -1 || mBoardData[endX][endY] == -1) {
			return;
		}
		// 起点和终点是同一个方块不做操作
		if (startX == endX && startY == endY) {
			return;
		}
		// 不同的方块不做操作，相同的方块才有可能消除
		if (mBoardData[startX][startY] != mBoardData[endX][endY]) {
			return;
		}
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
				if (currX == endX && currY - 1 == endY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(endX, endY));
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
				if (currX == endX && currY + 1 == endY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(endX, endY));
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
				if (currX - 1 == endX && currY == endY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(endX, endY));
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
				if (currX + 1 == endX && currY == endY) {
					resultPath.insert(resultPath.end(), workList[0]->begin(), workList[0]->end());
					resultPath.push_back(std::make_pair(endX, endY));
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
	}

	void eliminateBlock(int startX, int startY) {
		std::vector<std::pair<int, int>> resultPath;
		
		searchConnectedPath(startX, startY, mCurrActiveX, mCurrActiveY, resultPath);
	
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
			Sleep(200);
		}

	}

	// 自动查找可消除的方块
	void autoSearch(std::vector<std::pair<int, int>>& result) {
		std::vector<std::pair<int, int>> resultPath;
		for (int startX = 0; startX < mBoardXMax; startX++) {
			for (int startY = 0; startY < mBoardYMax; startY++) {
				for (int endX = 0; endX < mBoardXMax; endX++) {
					for (int endY = 0; endY < mBoardYMax; endY++) {
						searchConnectedPath(startX, startY, endX, endY, resultPath);
						if (resultPath.size() >= 2) {
							result.push_back(resultPath.front());
							result.push_back(resultPath.back());
							return;
						}
					}
				}
			}
		}
	}

	void autoEliminate() {
		std::vector<std::pair<int, int>> result;
		autoSearch(result);
		if (result.size() == 0) {
			MessageBox(GetHWnd(), "找不到啊\n重排一下试试吧", "结果", MB_OK | MB_ICONQUESTION);
			return;
		}
		drawActiveBlock(result[0].first, result[0].second, BLUE);
		drawActiveBlock(result[1].first, result[1].second, BLUE);
	}

	void processBoardClick(int mouseX, int mouseY) {
		if (checkMouseInBoard(mouseX, mouseY) == false) {
			return;
		}
		int xIndex = 0;
		int yIndex = 0;
		calcBoardClickXYIndex(mouseX, mouseY, xIndex, yIndex);
		// 连续两次点击相同方块，不做处理
		if (mCurrActiveX == xIndex && mCurrActiveY == yIndex) {
			return;
		}
		// 初次点击面板，初始化
		FlushBatchDraw();
		EndBatchDraw();

		eliminateBlock(xIndex, yIndex);

		BeginBatchDraw();
		mCurrActiveX = xIndex;
		mCurrActiveY = yIndex;
		// 注意，在格子数很多时，每次都重画会有极大的性能开销
		// 后续再做优化吧
		drawBoard();
		drawActiveBlock(mCurrActiveX, mCurrActiveY, RED);
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		BeginBatchDraw();
		if (eventIndex == -1) {
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 36) {
			// mCountDown.startCounting();
			initBoardData();
			drawBoard();
		} else if (eventIndex == 37) {
			shuffleBoard();
			drawBoard();
		} else if (eventIndex == 38) {
			drawBoard();
			autoEliminate();
		}
		FlushBatchDraw();
		EndBatchDraw();
	}

	virtual void processSomethingInLoop() override {
		mCountDown.doCounting();
	}
};