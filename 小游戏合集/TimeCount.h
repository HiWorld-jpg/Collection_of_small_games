#pragma once
#include <time.h>
#include <graphics.h>
#include <stdio.h>

class CountDown {
private:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	COLORREF mBkColor;
	COLORREF mFontColor;
	bool mEnable = false;
	bool mIsCounting = false;   // 倒计时是否结束
	unsigned mAllSeconds = 0;   // 倒计时开始时的初始秒
	unsigned mCurrSeconds = 0;
	time_t mCountStart = { 0 };

public:
	CountDown(int x, int y, int width, int height, COLORREF bkColor, COLORREF fontColor, unsigned allSeconds) {
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
		mAllSeconds = allSeconds;
		mBkColor = bkColor;
		mFontColor = fontColor;
	}

	void startCounting() {
		mEnable = true;
		mIsCounting = true;
		mCurrSeconds = mAllSeconds;
		mCountStart = time(nullptr);
		draw();
	}

	void stopCounting() {

	}

	void setEnable(bool enable) {
		mEnable = enable;
	}

	void doCounting() {
		if (mEnable == false || mIsCounting == false) {
			return;
		}
		time_t currTime = time(nullptr);
		time_t interval = currTime - mCountStart;
		if (interval > mAllSeconds) {
			mCurrSeconds = 0;
			return;
		}
		if (mCurrSeconds == mAllSeconds - interval) { // 一秒以内，不需要更新，防止draw调用次数过多
			return;
		}
		mCurrSeconds = mAllSeconds - interval;
		draw();
	}

	void clear() const {
		if (mEnable == false || mIsCounting == false) {
			return;
		}
		setfillcolor(mBkColor);
		solidrectangle(mX, mY, mX + mWidth, mY + mHeight);
	}

	void draw() const {
		if (mEnable == false || mIsCounting == false) {
			return;
		}
		clear();
		int minutes = mCurrSeconds / 60;
		int seconds = mCurrSeconds - minutes * 60;
		setlinecolor(mFontColor);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		rectangle(mX, mY, mX + mWidth, mY + mHeight);
		char temp[20] = { 0 };
		snprintf(temp, 20 - 1, "%d:%.2d", minutes, seconds);
		int textLen = strlen(temp);
		int fontHeight = mHeight * 0.8;
		int fontWidth = mWidth / textLen;
		settextstyle(fontHeight, fontWidth, _T("Consolas"));
		setbkmode(TRANSPARENT);
		settextcolor(mFontColor);
		outtextxy(mX, mY + mHeight * 0.1, temp);
	}
};