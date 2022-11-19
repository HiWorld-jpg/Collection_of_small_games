#pragma once
#include "Frame.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "FunctionUtils.h"
#include "FileDialog.h"
#include <stdio.h>
#include "NumberPad.h"

class BoardData {
private:
	int mValue[9][9];          // 0代表未填充数值，1-9表示填入的数
	int mValueCanBeWrite[9][9];  // 1代表可写，0表示不可写
	int mCurrActiveX = -1;
	int mCurrActiveY = -1;

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

	int getDataValue(int xIndex, int yIndex) const {
		if (xIndex < 0 || xIndex > 8) {
			return -1;
		}
		if (yIndex < 0 || yIndex > 8) {
			return -1;
		}
		return mValue[xIndex][yIndex];
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
		if (dataValue < 1 || dataValue > 9) {
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
			currTime->tm_mday + 1,
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
				fputc(mValueCanBeWrite[j][i] + '0', fp);
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
		int blockHeight = mBoardHeight / 9;
		int blockWidth = mBoardWidth / 9;
		char numToStr[2] = { 0 };
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				drawBlockInActive(i, j);
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
		setfillcolor(bkColor);
		solidrectangle(currX, currY, currX + blockWidth, currY + blockHeight);
		int currBoardValue = mBoardData.getDataValue(xIndex, yIndex);
		if (currBoardValue < 1 || currBoardValue > 9) {
			return;
		}
		char numToStr[2] = { 0 };
		snprintf(numToStr, 2, "%d", currBoardValue);
		numToStr[1] = '\0';
		FunctionUtils::printStrToRectangleArea(currX, currY, blockWidth, blockHeight, numToStr, fontColor);
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == -1) {  // 没有按到按钮
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 11) { // 按到了载入按钮
			mBoardData.readDataFromFile();
			drawBoard();
		} else if (eventIndex == 12) {  // 按道理存储按钮
			mBoardData.storeDataToFile();
		} else if (eventIndex == 13) {  // 按到了数字板
			int numberPadRetNum = mNumberPad->getPressedNumber(mouseX, mouseY);
			mBoardData.setDataValue(mBoardData.getActiveX(), mBoardData.getActiveY(), numberPadRetNum);
			drawBlock(mBoardData.getActiveX(), mBoardData.getActiveY());
		}
	}

	void processBoardClick(int mouseX, int mouseY) {
		if (checkMouseInBoard(mouseX, mouseY) == false) {
			return;
		}
		int xIndex = 0;
		int yIndex = 0;

		BeginBatchDraw();

		calcBoardClickXYIndex(mouseX, mouseY, xIndex, yIndex);
		int preActiveX = mBoardData.getActiveX();
		int preActiveY = mBoardData.getActiveY();
		mBoardData.updateActive(xIndex, yIndex);
		drawBlock(preActiveX, preActiveY);
		drawBlock(xIndex, yIndex);
		drawBoardLine();

		FlushBatchDraw();
		EndBatchDraw();
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

};