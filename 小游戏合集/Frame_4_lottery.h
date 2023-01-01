#pragma once
#include "Frame.h"
#include <time.h>

class Frame_4_lottery : public Frame {
private:
	int mCurrDaLeTou_01_35[35] = { 0 };
	int mCurrDaLeTou_01_12[12] = { 0 };
	int mCurrShuangSeQiu_1_33[33] = { 0 };
	int mCurrShuangSeQiu_1_16[16] = { 0 };
	static enum MODE {
		daLeTou,
		shuangSeQiu
	};
	MODE mCurrMode = MODE::daLeTou;

public:
	Frame_4_lottery(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
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

	void initDaLeTouNums() {
		for (int i = 0; i < 35; i++) {
			mCurrDaLeTou_01_35[i] = i + 1;
		}
		for (int i = 0; i < 12; i++) {
			mCurrDaLeTou_01_12[i] = i + 1;
		}
	}

	void initShuangSeQiuNums() {
		for (int i = 0; i < 33; i++) {
			mCurrShuangSeQiu_1_33[i] = i + 1;
		}
		for (int i = 0; i < 16; i++) {
			mCurrShuangSeQiu_1_16[i] = i + 1;
		}
	}

	// Knuth Algorithm
	void generateDaLeTouNums() {
		initDaLeTouNums();
		for (int i = 35; i >= 1; i--) {
			int currRandIndex = rand() % i;
			int temp = mCurrDaLeTou_01_35[currRandIndex];
			mCurrDaLeTou_01_35[currRandIndex] = mCurrDaLeTou_01_35[i - 1];
			mCurrDaLeTou_01_35[i - 1] = temp;
		}
		for (int i = 12; i >= 1; i--) {
			int currRandIndex = rand() % i;
			int temp = mCurrDaLeTou_01_12[currRandIndex];
			mCurrDaLeTou_01_12[currRandIndex] = mCurrDaLeTou_01_12[i - 1];
			mCurrDaLeTou_01_12[i - 1] = temp;
		}
	}

	// Knuth Algorithm
	void generateShuangSeQiuNums() {
		initShuangSeQiuNums();
		for (int i = 33; i >= 1; i--) {
			int currRandIndex = rand() % i;
			int temp = mCurrShuangSeQiu_1_33[currRandIndex];
			mCurrShuangSeQiu_1_33[currRandIndex] = mCurrShuangSeQiu_1_33[i - 1];
			mCurrShuangSeQiu_1_33[i - 1] = temp;
		}
		for (int i = 16; i >= 1; i--) {
			int currRandIndex = rand() % i;
			int temp = mCurrShuangSeQiu_1_16[currRandIndex];
			mCurrShuangSeQiu_1_16[currRandIndex] = mCurrShuangSeQiu_1_16[i - 1];
			mCurrShuangSeQiu_1_16[i - 1] = temp;
		}
	}

	void drawBall(int x, int y, int num, COLORREF numColor, COLORREF backColor, int ballRadius, bool zeroFormat) {
		setfillcolor(backColor);
		solidcircle(x, y, ballRadius);
		setbkmode(TRANSPARENT);
		char numToStr[5] = { 0 };
		if (zeroFormat) {
			sprintf(numToStr, "%.2d", num);
		} else {
			sprintf(numToStr, "%d", num);
		}
		settextcolor(numColor);
		int numHeight = ballRadius * 1.5;
		int numWidth = ballRadius / 2 * 1.5;

		settextstyle(numHeight, numWidth, _T("Consolas"));
		if (zeroFormat) {
			outtextxy(x - numWidth, y - numHeight / 2, numToStr);
		} else {
			if (num > 0 && num <= 9) {
				outtextxy(x - numWidth / 2, y - numHeight / 2, numToStr);
			} else {
				outtextxy(x - numWidth, y - numHeight / 2, numToStr);
			}
		}
	}

	void drawDaLeTouBalls(int x, int y, int width, int height, COLORREF backColor, COLORREF outlineColor) {
		setbkcolor(backColor);
		setbkmode(OPAQUE);
		clearrectangle(x, y, x + width, y + height);

		setlinecolor(outlineColor);
		setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
		rectangle(x, y, x + width, y + height);

		int ballRadius = width / (7 * 2 + 8 * 0.5);
		int gapOfBalls = ballRadius * 0.5;
		int firstBallCenterX = x + gapOfBalls + ballRadius;
		int firstBallCenterY = y + height / 2;
		for (int i = 0; i < 7; i++) {
			if (i < 5) {
				drawBall(firstBallCenterX + (ballRadius * 2 + gapOfBalls) * i, firstBallCenterY,
					mCurrDaLeTou_01_35[35 - 1 - i], WHITE, RGB(39, 119, 204), ballRadius, true);
			} else {
				drawBall(firstBallCenterX + (ballRadius * 2 + gapOfBalls) * i, firstBallCenterY,
					mCurrDaLeTou_01_12[12 - 1 - (i - 5)], BLACK, RGB(245, 199, 9), ballRadius, true);
			}
		}
	}

	void drawShuangSeQiuBalls(int x, int y, int width, int height, COLORREF backColor, COLORREF outlineColor) {
		setbkcolor(backColor);
		setbkmode(OPAQUE);
		clearrectangle(x, y, x + width, y + height);

		setlinecolor(outlineColor);
		setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
		rectangle(x, y, x + width, y + height);

		int ballRadius = width / (7 * 2 + 8 * 0.5);
		int gapOfBalls = ballRadius * 0.5;
		int firstBallCenterX = x + gapOfBalls + ballRadius;
		int firstBallCenterY = y + height / 2;
		for (int i = 0; i < 7; i++) {
			if (i < 6) {
				drawBall(firstBallCenterX + (ballRadius * 2 + gapOfBalls) * i, firstBallCenterY,
					mCurrShuangSeQiu_1_33[33 - 1 - i], WHITE, RGB(231, 38, 52), ballRadius, false);
			} else {
				drawBall(firstBallCenterX + (ballRadius * 2 + gapOfBalls) * i, firstBallCenterY,
					mCurrShuangSeQiu_1_16[16 - 1 - i], WHITE, RGB(40, 121, 237), ballRadius, false);
			}
		}
	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		BeginBatchDraw();

		if (eventIndex == 31) {
			if (mCurrMode == MODE::daLeTou) {
				mCurrMode = MODE::shuangSeQiu;
				Button* clickedButton = getButtonByEventIndex(eventIndex);
				if (clickedButton != nullptr) {
					clickedButton->setText((char*)"模式：双色球");
					init();
				}
			} else if (mCurrMode == MODE::shuangSeQiu) {
				mCurrMode = MODE::daLeTou;
				Button* clickedButton = getButtonByEventIndex(eventIndex);
				if (clickedButton != nullptr) {
					clickedButton->setText((char*)"模式：大乐透");
					init();
				}
			}
		} else if (eventIndex == 32) {
			if (mCurrMode == MODE::daLeTou) {
				init();
				for (int i = 0; i < 5; i++) {
					generateDaLeTouNums();
					drawDaLeTouBalls(20, 100 + 130 * i, 700, 90, WHITE, BLACK);
				}
			} else if (mCurrMode == MODE::shuangSeQiu) {
				init();
				for (int i = 0; i < 5; i++) {
					generateShuangSeQiuNums();
					drawShuangSeQiuBalls(20, 100 + 130 * i, 700, 90, WHITE, BLACK);
				}
			}
		} else if (eventIndex == 33) {
			if (mCurrMode == MODE::daLeTou) {
				init();
				IMAGE daLeTou_rule;
				loadimage(&daLeTou_rule, "./picture/daLeTou_rule.png", 400, 400 / 1.7);
				putimage(10, 70, &daLeTou_rule);
				IMAGE daLeTou_money;
				loadimage(&daLeTou_money, "./picture/daLeTou_money.png", 500, 500 * 1.3);
				putimage(400 + 30, 70, &daLeTou_money);

			} else if (mCurrMode == MODE::shuangSeQiu) {
				init();
				IMAGE shuangSeQiu_rule;
				// 放在资源文件中
				int rulePicHeight = 300;
				loadimage(&shuangSeQiu_rule, "./picture/shuangSeQiu_rule.png", rulePicHeight * 2.6, rulePicHeight);
				putimage(10, 70, &shuangSeQiu_rule);
				int moneyPicHeight = 400;
				IMAGE shuangSeQiu_money;
				loadimage(&shuangSeQiu_money, "./picture/shuangSeQiu_money.png", moneyPicHeight * 1.3, moneyPicHeight);
				putimage(10, rulePicHeight + 80, &shuangSeQiu_money);
			}
		}

		FlushBatchDraw();
		EndBatchDraw();
	}
};