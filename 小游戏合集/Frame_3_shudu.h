#pragma once
#include "Frame.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "FunctionUtils.h"
#include "FileDialog.h"
#include <stdio.h>
#include "NumberPad.h"
#include <Windows.h>

class BoardData {
private:
	int mValue[9][9];          // 0代表未填充数值，1-9表示填入的数
	int mValueCanBeWrite[9][9];  // 1代表可写，0表示不可写
	int mCurrActiveX = -1;
	int mCurrActiveY = -1;
	int mCurrMode;

public:
	static enum STATE {
		CONFLICT,
		VAILD_NOT_FINISHED,
		FINISHED
	    };

	static enum BOARD_MODE {
		PUZZLE_MODE,    // 解题模式下，mValueCanBeWrite值为0的格子不可修改，存储时格子的可写性不变
		FREE_MODE,      // 自由模式下，所有格子的值均可修改，存储时格子的可写性强制改为可写
		CREATE_MODE     // 创题模式下，所有格子的值均可修改，存储时如果格子里有值，那么可写性强制改为不可写，如果无值，则可写
	};

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

	void setBoardMode(BOARD_MODE mode) {
		mCurrMode = mode;
	}

	int getBoardMode() const {
		return mCurrMode;
	}

	int getDataValue(int xIndex, int yIndex) const {
		if (xIndex < 0 || xIndex > 8) {
			return -1;
		}
		if (yIndex < 0 || yIndex > 8) {
			return -1;
		}
		return mValue[xIndex][yIndex];
	}

	bool isBlockWritable(int xIndex, int yIndex) const {
		if (xIndex < 0 || xIndex > 8) {
			return false;
		}
		if (yIndex < 0 || yIndex > 8) {
			return false;
		}
		if (mValueCanBeWrite[xIndex][yIndex] == 0) {
			return false;
		} else if (mValueCanBeWrite[xIndex][yIndex] == 1) {
			return true;
		}
		return false;
	}

	int getActiveX() const { return mCurrActiveX; }

	int getActiveY() const { return mCurrActiveY; }

	void setDataValue(int xIndex, int yIndex, int dataValue) {
		if (xIndex < 0 || xIndex > 8) {
			return;
		}
		if (yIndex < 0 || yIndex > 8) {
			return;
		}
		if (dataValue < 0 || dataValue > 9) {
			return;
		}
		if (mCurrMode == BOARD_MODE::PUZZLE_MODE && isBlockWritable(xIndex, yIndex) == false) {
			return;
		}
		mValue[xIndex][yIndex] = dataValue;
	}

	void readDataFromFile() {
		char filePath[MyDialog::FILE_NAME_MAX_LENGTH] = { 0 };
		MyDialog::selectFileDialogBySuffix((char*)"txt file\0*.shudu\0\0", filePath);
		// 当前数独文件格式设置为22行
		char fileContent[22][50] = { 0 };
		if (checkFileFormat(filePath, fileContent) == false) {
			return;
		}
		for (int i = 2; i <= 10; i++) {
			int strLength = strlen(fileContent[i]);
			int numCount = 0;
			for (int j = 0; j < strLength; j++) {
				char c = fileContent[i][j];
				if (j % 2 == 0) {
					mValue[i - 2][numCount] = c - '0';
					numCount++;
				}
			}
		}
		for (int i = 12; i <= 20; i++) {
			int strLength = strlen(fileContent[i]);
			int numCount = 0;
			for (int j = 0; j < strLength; j++) {
				char c = fileContent[i][j];
				if (j % 2 == 0) {
					mValueCanBeWrite[i - 12][numCount] = c - '0';
					numCount++;
				}
			}
		}
	}

	bool checkFileFormat(char* filePath, char fileContent[][50]) {
		if (strlen(filePath) == 0) {
			return false;
		}
		FILE* fp = fopen(filePath, "r");
		if (fp == NULL) {
			return false;
		}
		int currLine = 0;
		while (fgets(fileContent[currLine], 50, fp) != NULL) {
			// fgets会把每行末尾的换行符也当作字符加入到字符串中
			int strLength = strlen(fileContent[currLine]);
			if (fileContent[currLine][strLength - 1] == '\n') {
				fileContent[currLine][strLength - 1] = '\0';
			}
			currLine++;
			if (currLine >= 22) {
				break;
			}
		}
		if (currLine != 22) {
			return false;
		}
		fclose(fp);

		// 调试用
		/*
		closegraph();
		for (int i = 0; i < 22; i++) {
			printf("%s\n", fileContent[i]);
		}
		printf("\n\n");
		*/
		
		if (strcmp(fileContent[0], (char*)"shudu file begin") != 0) {
			//printf("1\n");
			return false;
		}
		if (strcmp(fileContent[1], (char*)"value") != 0) {
			//printf("2\n");
			return false;
		}
		if (strcmp(fileContent[11], (char*)"writeable") != 0) {
			//printf("3\n");
			return false;
		}
		if (strcmp(fileContent[21], (char*)"shudu file end") != 0) {
			//printf("4\n");
			return false;
		}
		// 判断value的值是否是0 - 9，且数字与空格相互间隔
		for (int i = 2; i <= 10; i++) {
			int strLength = strlen(fileContent[i]);
			if (strLength == 0 || strLength > 17) {
				//printf("5\n");
				return false;
			}
			for (int j = 0; j < strLength; j++) {
				char c = fileContent[i][j];
				if (j % 2 == 0) {
					if (c < '0' || c > '9') {
						//printf("6\n");
						return false;
					}
				} else {
					if (c != ' ') {
						//printf("7\n");
						return false;
					}
				}
			}
		}
		// 判断writeable的值是否是0或1，且数字与空格相互间隔
		for (int i = 12; i <= 20; i++) {
			int strLength = strlen(fileContent[i]);
			if (strLength == 0 || strLength > 17) {
				//printf("8\n");
				return false;
			}
			for (int j = 0; j < strLength; j++) {
				char c = fileContent[i][j];
				if (j % 2 == 0) {
					if (c != '0' && c != '1') {
						//printf("9\n");
						return false;
					}
				} else {
					if (c != ' ') {
						//printf("10\n");
						return false;
					}
				}
			}
		}

		return true;
	}

	void storeDataToFile() {
		char dirName[MyDialog::PATH_MAX_LENGTH] = { 0 };
		MyDialog::selectDirDialog(dirName);
		if (strlen(dirName) == 0) {
			return;
		}
		time_t nowTime;
		time(&nowTime);
		struct tm* currTime = localtime(&nowTime);
		char fileName[100] = { 0 };
		snprintf(fileName, 100 - 1, "\\archive_%d%.2d%.2d_%.2d-%.2d-%.2d.shudu",
			currTime->tm_year + 1900,
			currTime->tm_mon + 1,
			currTime->tm_mday,
			currTime->tm_hour,
			currTime->tm_min,
			currTime->tm_sec);
		char fileAbsolutePath[MyDialog::PATH_MAX_LENGTH + 100] = { 0 };
		int fileAbsolutePathLength = 0;
		int dirNameLength = strlen(dirName);
		for (int i = 0; i < dirNameLength; i++) {
			if (dirName[i] != '\\') {
				fileAbsolutePath[fileAbsolutePathLength++] = dirName[i];
				continue;
			}
			fileAbsolutePath[fileAbsolutePathLength++] = '\\';
		}
		snprintf(fileAbsolutePath, MyDialog::PATH_MAX_LENGTH + 100 - 1, "%s%s\0", dirName, fileName);

		// 调试用
		/*closegraph();
		printf("dirName=%s\n", dirName);
		printf("fileName=%s\n", fileName);
		printf("fileAbsolutePath=%s\n", fileAbsolutePath);*/

		FILE* fp = fopen(fileAbsolutePath, "w");
		if (fp == NULL) {
			printf("fail to open\n");
			return;
		}
		printf("succees to open\n");

		fputs("shudu file begin\n", fp);
		fputs("value\n", fp);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				fputc(mValue[j][i] + '0', fp);
				if (j == 8) {
					fputc('\n', fp);
				} else {
					fputc(' ', fp);
				}
			}
		}
		fputs("writeable\n", fp);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (mCurrMode == BOARD_MODE::PUZZLE_MODE) {
					fputc(mValueCanBeWrite[j][i] + '0', fp);
				} else if (mCurrMode == BOARD_MODE::FREE_MODE) {
					fputc('1', fp);
				} else if (mCurrMode == BOARD_MODE::CREATE_MODE) {
					if (mValue[j][i] == 0) {
						fputc('1', fp);
					} else if (mValue[j][i] >= 1 && mValue[j][i] <= 9) {
						fputc('0', fp);
					}
				}
				
				if (j == 8) {
					fputc('\n', fp);
				} else {
					fputc(' ', fp);
				}
			}
		}
		fputs("shudu file end", fp);
		fclose(fp);
	}

	void setCurrActiveXY(int xIndex, int yIndex) {
		mCurrActiveX = xIndex;
		mCurrActiveY = yIndex;
	}

	void updateActive(int xIndex, int yIndex) {
		setCurrActiveXY(xIndex, yIndex);
	}

	bool checkWorkListValid(std::vector<std::pair<int, int>>* workList, std::vector<std::pair<int, int>>* conflictPos) {
		// 只要这个值不是0，且其他值没有和这个值重复，那么这个值就是合理的
		for (auto begin = workList->begin(), end = workList->end(); begin != end; ++begin) {
			int currDataValue = mValue[begin->first][begin->second];
			if (currDataValue == 0) {
				continue;
			}
			for (auto afterBegin = begin + 1; afterBegin != end; ++afterBegin) {
				int comparedValue = mValue[afterBegin->first][afterBegin->second];
				if (currDataValue == 0) {
					continue;
				}
				if (currDataValue == comparedValue) {
					if (conflictPos != nullptr) {
						conflictPos->push_back(std::make_pair(begin->first, begin->second));
						conflictPos->push_back(std::make_pair(afterBegin->first, afterBegin->second));
					}
					return false;
				}
			}
		}
		return true;
	}

	int checkBoardState(std::vector<std::pair<int, int>> *conflictPos = nullptr) {
		std::vector<std::pair<int, int>> workList;
		// 先看每列是否合理
		for (int i = 0; i < 9; i++) {
			workList.clear();
			for (int j = 0;j < 9; j++) {
				workList.push_back(std::make_pair(i, j));
			}
			if (checkWorkListValid(&workList, conflictPos) == false) {
				return STATE::CONFLICT;
			}
		}
		// 再看每行是否合理
		for (int i = 0; i < 9; i++) {
			workList.clear();
			for (int j = 0;j < 9; j++) {
				workList.push_back(std::make_pair(j, i));
			}
			if (checkWorkListValid(&workList, conflictPos) == false) {
				return STATE::CONFLICT;
			}
		}
		// 再看每个九宫格是否合理
		for (int i = 0; i < 8; i += 3) {
			for (int j = 0; j < 8; j += 3) {
				workList.clear();
				for (int ii = 0; ii < 3; ii++) {
					for (int jj = 0; jj < 3; jj++) {
						workList.push_back(std::make_pair(i + ii, j + jj));
					}
				}
				if (checkWorkListValid(&workList, conflictPos) == false) {
					return STATE::CONFLICT;
				}
			}
		}
		// 再看是否填满，如果满了，那么说明数独成功完成
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (mValue[i][j] == 0) {
					return STATE::VAILD_NOT_FINISHED;
				}
			}
		}

		return STATE::FINISHED;
	}

	void resetBoardData() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (mCurrMode == BoardData::BOARD_MODE::PUZZLE_MODE) {
					if (isBlockWritable(i, j) == true) {
						setDataValue(i, j, 0);
					}
				} else if (mCurrMode == BoardData::BOARD_MODE::FREE_MODE ||
					getBoardMode() == BoardData::BOARD_MODE::CREATE_MODE) {
					setDataValue(i, j, 0);
				}
			}
		}
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
	NumberPad* mNumberPad = nullptr;
	bool mEnableAnimation = true;

public:
	Frame_3_shudu(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex,
		int boardX, int boardY, int boardWidth, int boardHeight, 
		COLORREF boardBkColor, COLORREF boardOutlineColor, NumberPad *numberPad) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		mBoardX = boardX;
		mBoardY = boardY;
		mBoardWidth = boardWidth;
		mBoardHeight = boardHeight;
		mBoardBkColor = boardBkColor;
		mBoardOutlineColor = boardOutlineColor;
		mNumberPad = numberPad;
		mBoardData.setBoardMode(BoardData::BOARD_MODE::PUZZLE_MODE);
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
		drawBoard();
		mTextList[0]->draw();
	}

	void drawBoard() {
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
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				drawBlock(i, j);
			}
		}
	}

	void drawBlock(int xIndex, int yIndex) const {
		if (xIndex == mBoardData.getActiveX() &&
			yIndex == mBoardData.getActiveY()) {
			drawBlockActive(xIndex, yIndex);
		} else {
			drawBlockInActive(xIndex, yIndex);
		}
	}

	void drawBlockActive(int xIndex, int yIndex) const {
		drawBlockWithBkColor(xIndex, yIndex, LIGHTGREEN, BLACK);
	}

	void drawBlockInActive(int xIndex, int yIndex) const {
		drawBlockWithBkColor(xIndex, yIndex, mBoardBkColor, BLACK);
	}

	void drawBlockWithBkColor(int xIndex, int yIndex, COLORREF bkColor, COLORREF fontColor) const {
		if (checkXYIndexValid(xIndex, yIndex) == false) {
			return;
		}
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		int currX = mBoardX + xIndex * blockWidth;
		int currY = mBoardY + yIndex * blockHeight;
		int gap = 3; // 防止清除格子边框
		setfillcolor(bkColor);
		solidrectangle(currX + gap, currY + gap, currX + blockWidth - gap, currY + blockHeight - gap);
		int currBoardValue = mBoardData.getDataValue(xIndex, yIndex);
		if (currBoardValue <= 0 || currBoardValue > 9) {
			return;
		}
		char numToStr[2] = { 0 };
		snprintf(numToStr, 2, "%d", currBoardValue);
		numToStr[1] = '\0';
		FunctionUtils::printStrToRectangleArea(currX, currY, blockWidth, blockHeight, numToStr, fontColor);
		if (mBoardData.getBoardMode() == BoardData::BOARD_MODE::PUZZLE_MODE && mBoardData.isBlockWritable(xIndex, yIndex) == false) {
			drawWritable(xIndex, yIndex, fontColor);
		}
	}

	void drawWritable(int xIndex, int yIndex, COLORREF fontColor) const {
		if (checkXYIndexValid(xIndex, yIndex) == false) {
			return;
		}
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		int currX = mBoardX + (xIndex + 1) * blockWidth;  // 定位到格子右下角
		int currY = mBoardY + (yIndex + 1) * blockHeight;
		int lockBodyLeftUpX = currX - 10;
		int lockBodyLeftUpY = currY - 9;
		int lockBodyRightDownX = currX - 4;
		int lockBodyRightDownY = currY - 4;
		setfillcolor(BLACK);
		solidrectangle(lockBodyLeftUpX, lockBodyLeftUpY, lockBodyRightDownX, lockBodyRightDownY);
		setlinecolor(fontColor);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		int lockLineLength = 2;
		int lockLineOffset = 1;
		line(lockBodyLeftUpX + lockLineOffset, lockBodyLeftUpY, lockBodyLeftUpX + lockLineOffset, lockBodyLeftUpY - lockLineLength);
		line(lockBodyRightDownX - lockLineOffset, lockBodyLeftUpY, lockBodyRightDownX - lockLineOffset, lockBodyLeftUpY - lockLineLength);
		circle((lockBodyLeftUpX + lockBodyRightDownX) / 2, lockBodyLeftUpY - lockLineLength, (lockBodyRightDownX - lockBodyLeftUpX) / 2 - lockLineOffset);
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		BeginBatchDraw();

		if (eventIndex == -1) {  // 没有按到按钮
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 11) { // 按到了载入按钮
			mBoardData.readDataFromFile();
			drawBoard();
		} else if (eventIndex == 12) {  // 按到了存储按钮
			mBoardData.storeDataToFile();
		} else if (eventIndex == 13) {  // 按到了数字板
			int numberPadRetNum = mNumberPad->getPressedNumber(mouseX, mouseY);
			mBoardData.setDataValue(mBoardData.getActiveX(), mBoardData.getActiveY(), numberPadRetNum);
			drawBoard();
		} else if (eventIndex == 22) { // 擦除
			mBoardData.setDataValue(mBoardData.getActiveX(), mBoardData.getActiveY(), 0);
			drawBoard();
		} else if (eventIndex == 23) { // 重置
			int inputBoxRet = MessageBox(GetHWnd(), "是否继续清空所有已填内容", "危险操作", MB_YESNO | MB_ICONQUESTION);
			if (inputBoxRet == IDYES) {
				mBoardData.resetBoardData();
				drawBoard();
			}
		} else if (eventIndex == 24) {  // 检查
			drawBoard();
			checkBoardValid();
		} else if (eventIndex == 25) {  // 模式
			if (mBoardData.getBoardMode() == BoardData::BOARD_MODE::PUZZLE_MODE) {
				mBoardData.setBoardMode(BoardData::BOARD_MODE::FREE_MODE);
				char tempStr[20] = { "自由模式" };
				Button* currButton = getButtonByEventIndex(eventIndex);
				if (currButton != nullptr) {
					currButton->setText(tempStr);
					currButton->clearButtonText();
					currButton->drawButtonText();
					drawBoard();
				}
			} else if (mBoardData.getBoardMode() == BoardData::BOARD_MODE::FREE_MODE) {
				mBoardData.setBoardMode(BoardData::BOARD_MODE::CREATE_MODE);
				char tempStr[20] = { "创题模式" };
				Button* currButton = getButtonByEventIndex(eventIndex);
				if (currButton != nullptr) {
					currButton->setText(tempStr);
					currButton->clearButtonText();
					currButton->drawButtonText();
					drawBoard();
				}
			} else if (mBoardData.getBoardMode() == BoardData::BOARD_MODE::CREATE_MODE) {
				mBoardData.setBoardMode(BoardData::BOARD_MODE::PUZZLE_MODE);
				char tempStr[20] = { "解题模式" };
				Button* currButton = getButtonByEventIndex(eventIndex);
				if (currButton != nullptr) {
					currButton->setText(tempStr);
					currButton->clearButtonText();
					currButton->drawButtonText();
					drawBoard();
				}
			}
		} else if (eventIndex == 26) { // 最大值解法
			if (checkBoardValid() == true) {
				mTextList[3]->setText((char*)"耗时0分0秒0毫秒");
				mTextList[3]->draw();
				FlushBatchDraw();

				SYSTEMTIME timeStart = { 0 };
				SYSTEMTIME timeEnd = { 0 };
				GetLocalTime(&timeStart);

				maxSolve();

				GetLocalTime(&timeEnd);
				unsigned int timeEndMilliSeconds = timeEnd.wMinute * 60 * 1000 + timeEnd.wSecond * 1000 + timeEnd.wMilliseconds;
				unsigned int timeStartMilliSeconds = timeStart.wMinute * 60 * 1000 + timeStart.wSecond * 1000 + timeStart.wMilliseconds;
				unsigned int allMilliSeconds = timeEndMilliSeconds - timeStartMilliSeconds;
				WORD minuteCost = allMilliSeconds / (60 * 1000);
				WORD secondCost = (allMilliSeconds - minuteCost * 60 * 1000) / 1000;
				WORD milliSecondCost = allMilliSeconds - minuteCost * 60 * 1000 - secondCost * 1000;
				char tempStr2[50] = { 0 };
				snprintf(tempStr2, 50 - 1, "耗时%u分%u秒%u毫秒", minuteCost, secondCost, milliSecondCost);

				mTextList[3]->setText(tempStr2);
				mTextList[3]->draw();
				drawBoard();
				checkBoardValid();
			}
			
		} else if (eventIndex == 27) { // 最小值解法
			if (checkBoardValid() == true) {
				mTextList[3]->setText((char*)"耗时0分0秒0毫秒");
				mTextList[3]->draw();
				FlushBatchDraw();

				SYSTEMTIME timeStart = { 0 };
				SYSTEMTIME timeEnd = { 0 };
				GetLocalTime(&timeStart);

				minSolve();

				GetLocalTime(&timeEnd);
				unsigned int timeEndMilliSeconds = timeEnd.wMinute * 60 * 1000 + timeEnd.wSecond * 1000 + timeEnd.wMilliseconds;
				unsigned int timeStartMilliSeconds = timeStart.wMinute * 60 * 1000 + timeStart.wSecond * 1000 + timeStart.wMilliseconds;
				unsigned int allMilliSeconds = timeEndMilliSeconds - timeStartMilliSeconds;
				WORD minuteCost = allMilliSeconds / (60 * 1000);
				WORD secondCost = (allMilliSeconds - minuteCost * 60 * 1000) / 1000;
				WORD milliSecondCost = allMilliSeconds - minuteCost * 60 * 1000 - secondCost * 1000;
				char tempStr[50] = { 0 };
				snprintf(tempStr, 50 - 1, "耗时%u分%u秒%u毫秒", minuteCost, secondCost, milliSecondCost);

				mTextList[3]->setText(tempStr);
				mTextList[3]->draw();
				drawBoard();
				checkBoardValid();
			}
			
		} else if (eventIndex == 28) { // 动画
			if (mEnableAnimation == true) {
				mEnableAnimation = false;
				char tempStr[10] = { "动画: 关" };
				Button* currButton = getButtonByEventIndex(eventIndex);
				if (currButton != nullptr) {
					currButton->setText(tempStr);
					currButton->clearButtonText();
					currButton->drawButtonText();
					drawBoard();
				}
			} else {
				mEnableAnimation = true;
				char tempStr[10] = { "动画: 开" };
				Button* currButton = getButtonByEventIndex(eventIndex);
				if (currButton != nullptr) {
					currButton->setText(tempStr);
					currButton->clearButtonText();
					currButton->drawButtonText();
					drawBoard();
				}
			}
		}

		FlushBatchDraw();
		EndBatchDraw();
	}

	void processBoardClick(int mouseX, int mouseY) {
		if (checkMouseInBoard(mouseX, mouseY) == false) {
			return;
		}
		int xIndex = 0;
		int yIndex = 0;

		calcBoardClickXYIndex(mouseX, mouseY, xIndex, yIndex);
		int preActiveX = mBoardData.getActiveX();
		int preActiveY = mBoardData.getActiveY();
		mBoardData.updateActive(xIndex, yIndex);
		drawBlock(preActiveX, preActiveY);
		drawBlock(xIndex, yIndex);
		drawBoardLine();
	}

	virtual int processMouseClickUp(int mouseX, int mouseY) override {
		int clickedMouseIndex = -1;
		for (int i = 0; i < mButtonNum; i++) {
			if (mButtonList[i]->checkMouseIn(mouseX, mouseY) == true) {
				mButtonList[i]->drawButtonUp(mouseX, mouseY);
				clickedMouseIndex = mButtonList[i]->getGlobalIndex();
				break;
			}
		}
		processEvent(clickedMouseIndex, mouseX, mouseY);
		return clickedMouseIndex;
	}

	void calcBoardClickXYIndex(int mouseX, int mouseY, int& xIndex, int& yIndex) const {
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		xIndex = (mouseX - mBoardX) / blockWidth;
		yIndex = (mouseY - mBoardY) / blockHeight;
	}

	bool checkMouseInBoard(int mouseX, int mouseY) const {
		if (mouseX <= mBoardX || mouseX >= mBoardX + mBoardWidth) {
			return false;
		}
		if (mouseY <= mBoardY || mouseY >= mBoardY + mBoardHeight) {
			return false;
		}
		return true;
	}

	bool checkXYIndexValid(int xIndex, int yIndex) const {
		if (xIndex < 0 || xIndex > 8) {
			return false;
		}
		if (yIndex < 0 || yIndex > 8) {
			return false;
		}
		return true;
	}

	bool checkBoardValid() {
		std::vector<std::pair<int, int>> conflictPos;
		int checkResult = mBoardData.checkBoardState(&conflictPos);
		if (checkResult == BoardData::STATE::VAILD_NOT_FINISHED) {
			mTextList[0]->draw();
			return true;
		} else if (checkResult == BoardData::STATE::FINISHED) {
			mTextList[1]->draw();
			return true;
		} else if (checkResult == BoardData::STATE::CONFLICT) {
			mTextList[2]->draw();
			if (conflictPos.size() == 2) {
				drawBlockWithBkColor(conflictPos[0].first, conflictPos[0].second, RED, BLACK);
				drawBlockWithBkColor(conflictPos[1].first, conflictPos[1].second, RED, BLACK);
			}
			return false;
		}
		return false;
	}

	bool minSolve() {
		int currXIndex = -1;
		int currYIndex = -1;
		bool isFoundEmptyBlock = false;
		for (int i = 0; i < 9; i++) {
			isFoundEmptyBlock = false;
			for (int j = 0; j < 9; j++) {
				if (mBoardData.getDataValue(j, i) == 0) {
					currXIndex = j;
					currYIndex = i;
					isFoundEmptyBlock = true;
					break;
				}
			}
			if (isFoundEmptyBlock == true) {
				break;
			}
		}

		if (isFoundEmptyBlock == false) {
			return false;
		}

		for (int i = 1; i <= 9; i++) {
			mBoardData.setDataValue(currXIndex, currYIndex, i);
			if (mEnableAnimation == true) {
				drawBlock(currXIndex, currYIndex);
				FlushBatchDraw();
			}
			int currState = mBoardData.checkBoardState();
			if (currState == BoardData::STATE::CONFLICT) {
				continue;
			} else if (currState == BoardData::STATE::VAILD_NOT_FINISHED) {
				bool result = minSolve();
				if (result == true) {
					return true;
				}
			} else if (currState == BoardData::STATE::FINISHED) {
				return true;
			}
		}
		mBoardData.setDataValue(currXIndex, currYIndex, 0);
		if (mEnableAnimation == true) {
			drawBlock(currXIndex, currYIndex);
			FlushBatchDraw();
		}

		return false;
	}

	bool maxSolve() {
		int currXIndex = -1;
		int currYIndex = -1;
		bool isFoundEmptyBlock = false;
		for (int i = 0; i < 9; i++) {
			isFoundEmptyBlock = false;
			for (int j = 0; j < 9; j++) {
				if (mBoardData.getDataValue(j, i) == 0) {
					currXIndex = j;
					currYIndex = i;
					isFoundEmptyBlock = true;
					break;
				}
			}
			if (isFoundEmptyBlock == true) {
				break;
			}
		}

		if (isFoundEmptyBlock == false) {
			return false;
		}

		for (int i = 9; i >= 1; i--) {
			mBoardData.setDataValue(currXIndex, currYIndex, i);
			if (mEnableAnimation == true) {
				drawBlock(currXIndex, currYIndex);
				FlushBatchDraw();
			}
			int currState = mBoardData.checkBoardState();
			if (currState == BoardData::STATE::CONFLICT) {
				continue;
			} else if (currState == BoardData::STATE::VAILD_NOT_FINISHED) {
				bool result = minSolve();
				if (result == true) {
					return true;
				}
			} else if (currState == BoardData::STATE::FINISHED) {
				return true;
			}
		}
		mBoardData.setDataValue(currXIndex, currYIndex, 0);
		if (mEnableAnimation == true) {
			drawBlock(currXIndex, currYIndex);
			FlushBatchDraw();
		}

		return false;
	}
};