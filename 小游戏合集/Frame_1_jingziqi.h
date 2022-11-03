#pragma once
#include "Frame.h"

class Frame_1_jingziqi : public Frame {
private:
	int mBoardX;
	int mBoardY;
	int mBoardWidth;
	int mBoardHeight;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;
	char mHookPicPath[100] = { 0 };
	char mForkPicPath[100] = { 0 };
	int mBoardData[3][3]; // 0表示格子还没有玩家落子，1表示玩家1已在这个格子落子，-1表示玩家2，-2表示已有玩家赢了，重玩之前格子不允许再下
	int mCurrPlayer = 1;   // 1表示当前是玩家1落子，-1表示玩家2
	int mScore[3]; // 分别表示玩家1赢，平局，玩家2赢
public:
	Frame_1_jingziqi(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex, 
		int boardX, int boardY, int boardWidth, int boardHeight, COLORREF boardBkColor, COLORREF boardOutlineColor) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		mBoardX = boardX;
		mBoardY = boardY;
		mBoardWidth = boardWidth;
		mBoardHeight = boardHeight;
		mBoardBkColor = boardBkColor;
		mBoardOutlineColor = boardOutlineColor;
		initScore();
	}
	
	virtual void init() override {
		BeginBatchDraw();
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
		
		initBoard();

		FlushBatchDraw();
		EndBatchDraw();
	}

	void initBoardData() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				mBoardData[i][j] = 0;
			}
		}
	}

	void initBoard() {
		mCurrPlayer = 1;
		initBoardData();
		setfillcolor(mBoardBkColor);
		solidrectangle(mBoardX, mBoardY, mBoardX + mBoardWidth, mBoardY + mBoardHeight);
		drawBoardLine();
		clearHintText();
		char str[30] = { "现在轮到玩家1落子" };
		settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
		settextcolor(GREEN);
		outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str);
		drawScore();
	}

	void initScore() {
		for (int i = 0; i < 3; i++) {
			mScore[i] = 0;
		}
	}

	void drawBoardLine() const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		setlinecolor(mBoardOutlineColor);

		int blockHeight = mBoardHeight / 3;
		int blockWidth = mBoardWidth / 3;
		for (int i = 0; i < 4; i++) {
			line(mBoardX, mBoardY + i * blockHeight, mBoardX + mBoardWidth, mBoardY + i * blockHeight);
		}
		for (int i = 0; i < 4; i++) {
			line(mBoardX + i * blockWidth, mBoardY, mBoardX + i * blockWidth, mBoardY + mBoardHeight);
		}
	}
	
	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == -1) {
			processBoardClick(mouseX, mouseY);
		} else if (eventIndex == 2) {
			initBoard();
		}
	}

	void setBoardDataLocked() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (mBoardData[i][j] == 0) {
					mBoardData[i][j] = -2;
				}
			}
		}
	}

	void clearHintText() {
		setfillcolor(mBoardBkColor);
		solidrectangle(mBoardX, mBoardY + 1.05 * mBoardHeight,
			mBoardX + mBoardWidth, mBoardY + 1.05 * mBoardHeight + mBoardWidth / 15 * 2);
	}

	void processBoardClick(int mouseX, int mouseY) {
		if (mouseX <= mBoardX || mouseX >= mBoardX + mBoardWidth) {
			return;
		}
		if (mouseY <= mBoardY || mouseY >= mBoardY + mBoardHeight) {
			return;
		}
		int blockHeight = mBoardHeight / 3;
		int blockWidth = mBoardWidth / 3;

		int xIndex = (mouseX - mBoardX) / blockWidth;
		int yIndex = (mouseY - mBoardY) / blockHeight;

		if (mBoardData[xIndex][yIndex] == 0) {
			char str1[30] = { "现在轮到玩家1落子" };
			char str2[30] = { "现在轮到玩家2落子" };
			char str3[30] = { "游戏结束，玩家1赢了" };
			char str4[30] = { "游戏结束，平局" };
			char str5[30] = { "游戏结束，玩家2赢了" };

			mBoardData[xIndex][yIndex] = mCurrPlayer;
			BeginBatchDraw();
			if (mCurrPlayer == 1) {
				drawHookDefault(xIndex, yIndex);
				drawBoardLine();
				mCurrPlayer *= -1;
				clearHintText();
				setbkmode(TRANSPARENT);
				settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
				settextcolor(RED);
				outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str1);
			} else if (mCurrPlayer == -1) {
				drawForkDefault(xIndex, yIndex);
				drawBoardLine();
				mCurrPlayer *= -1;
				clearHintText();
				setbkmode(TRANSPARENT);
				settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
				settextcolor(GREEN);
				outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str2);
			}
			
			int result = checkGameResult();
			if (result == 1) {
				mScore[0]++;
				drawScore();
				setBoardDataLocked();
				clearHintText();
				setbkmode(TRANSPARENT);
				settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
				settextcolor(GREEN);
				outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str3);
			} else if (result == -1) {
				mScore[2]++;
				drawScore();
				setBoardDataLocked();
				clearHintText();
				setbkmode(TRANSPARENT);
				settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
				settextcolor(RED);
				outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str5);
			} else if (result == 2) {
				mScore[1]++;
				drawScore();
				setBoardDataLocked();
				clearHintText();
				setbkmode(TRANSPARENT);
				settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
				settextcolor(BLUE);
				outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight, str4);
			}

			FlushBatchDraw();
			EndBatchDraw();
		}
	}

	void drawHookDefault(int xIndex, int yIndex) {
		int blockHeight = mBoardHeight / 3;
		int blockWidth = mBoardWidth / 3;

		int currBlockX = mBoardX + xIndex * blockWidth;
		int currBlockY = mBoardY + yIndex * blockHeight;

		setfillcolor(mBoardBkColor);
		solidrectangle(currBlockX, currBlockY, currBlockX + blockWidth, currBlockY + blockHeight);

		setfillcolor(GREEN);

		int smallBlockWidth = blockWidth / 4;
		int smallBlockHeight = blockHeight / 4;

		POINT pts[] = { 
			{currBlockX + 1 * smallBlockWidth, currBlockY + 2.5 * smallBlockHeight}, 
			{currBlockX + 1 * smallBlockWidth, currBlockY + 3 * smallBlockHeight},
			{currBlockX + 2 * smallBlockWidth, currBlockY + 3.5 * smallBlockHeight},
			{currBlockX + 3 * smallBlockWidth, currBlockY + 2 * smallBlockHeight},
			{currBlockX + 3 * smallBlockWidth, currBlockY + 1 * smallBlockHeight},
			{currBlockX + 2 * smallBlockWidth, currBlockY + 3 * smallBlockHeight},
		};
		solidpolygon(pts, 6);
	}

	void drawForkDefault(int xIndex, int yIndex) {
		int blockHeight = mBoardHeight / 3;
		int blockWidth = mBoardWidth / 3;

		int currBlockX = mBoardX + xIndex * blockWidth;
		int currBlockY = mBoardY + yIndex * blockHeight;

		setfillcolor(mBoardBkColor);
		solidrectangle(currBlockX, currBlockY, currBlockX + blockWidth, currBlockY + blockHeight);

		setfillcolor(RED);

		int smallBlockWidth = blockWidth / 4;
		int smallBlockHeight = blockHeight / 4;

		POINT pts1[] = {
			{currBlockX + 0.7 * smallBlockWidth, currBlockY + 3 * smallBlockHeight},
			{currBlockX + 1 * smallBlockWidth, currBlockY + 3.3 * smallBlockHeight},
			{currBlockX + 3.3 * smallBlockWidth, currBlockY + 1 * smallBlockHeight},
			{currBlockX + 3 * smallBlockWidth, currBlockY + 0.7 * smallBlockHeight},
		};
		solidpolygon(pts1, 4);

		POINT pts2[] = {
			{currBlockX + 1 * smallBlockWidth, currBlockY + 0.7 * smallBlockHeight},
			{currBlockX + 3.3 * smallBlockWidth, currBlockY + 3 * smallBlockHeight},
			{currBlockX + 3 * smallBlockWidth, currBlockY + 3.3 * smallBlockHeight},
			{currBlockX + 0.7 * smallBlockWidth, currBlockY + 1 * smallBlockHeight},
		};
		solidpolygon(pts2, 4);
	}

	// 返回值0，表示谁都没赢且游戏未结束
	// 返回值1，表示玩家1赢了
	// 返回值-1，表示玩家2赢了
	// 返回值2，表示平局且游戏结束
	int checkGameResult() {
		int blockHeight = mBoardHeight / 3;
		int blockWidth = mBoardWidth / 3;

		for (int i = 0; i < 3; i++) {
			if (mBoardData[i][0] != 0 &&
				mBoardData[i][0] == mBoardData[i][1] &&
				mBoardData[i][0] == mBoardData[i][2]) {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 6);
				setlinecolor(BLUE);
				line(mBoardX + i * blockWidth + blockWidth / 2, mBoardY, 
					 mBoardX + i * blockWidth + blockWidth / 2, mBoardY + mBoardHeight);
				return mBoardData[i][0];
			}
		}
		for (int j = 0; j < 3; j++) {
			if (mBoardData[0][j] != 0 &&
				mBoardData[0][j] == mBoardData[1][j] &&
				mBoardData[0][j] == mBoardData[2][j]) {
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 6);
				setlinecolor(BLUE);
				line(mBoardX, mBoardY + j * blockHeight + blockHeight / 2,
					 mBoardX + mBoardWidth, mBoardY + j * blockHeight + blockHeight / 2);
				return mBoardData[0][j];
			}
		}
		if (mBoardData[0][0] != 0 &&
			mBoardData[0][0] == mBoardData[1][1] &&
			mBoardData[0][0] == mBoardData[2][2]) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 6);
			setlinecolor(BLUE);
			line(mBoardX + 1, mBoardY + 1, mBoardX + mBoardWidth - 1, mBoardY + mBoardHeight - 1);
			return mBoardData[0][0];
		}
		if (mBoardData[2][0] != 0 &&
			mBoardData[2][0] == mBoardData[1][1] &&
			mBoardData[2][0] == mBoardData[0][2]) {
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 6);
			setlinecolor(BLUE);
			line(mBoardX + mBoardWidth - 1, mBoardY + 1, mBoardX + 1, mBoardY + mBoardHeight - 1);
			return mBoardData[2][0];
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (mBoardData[i][j] == 0) {
					return 0;
				}
			}
		}
		return 2;
	}

	void drawScore() {
		char str1[30] = { "玩家1赢 : 平局 : 玩家2赢" };
		setbkmode(TRANSPARENT);
		settextstyle(mBoardWidth / 15 * 2, mBoardWidth / 15, _T("Fixedsys"));
		settextcolor(BLACK);
		outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight + mBoardWidth / 15, str1);
		char scoreString[30];
		snprintf(scoreString, 29, "  %d  :  %d  :  %d", mScore[0], mScore[1], mScore[2]);
		outtextxy(mBoardX, mBoardY + 1.05 * mBoardHeight + mBoardWidth / 15 * 2, scoreString);
	}
};