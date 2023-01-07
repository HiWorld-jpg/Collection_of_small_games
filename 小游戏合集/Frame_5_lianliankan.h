#pragma once
#include "Frame.h"
#include "TimeCount.h"

class Frame_5_lianliankan : public Frame {
private:
	CountDown mCountDown;
	

public:
	Frame_5_lianliankan(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex),
		mCountDown(100, 100, 200, 80, frameBkColor, BLACK, 20) {
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

	

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == 36) {
			mCountDown.startCounting();
		} else if (eventIndex == 37) {

		}
	}

	virtual void processSomethingInLoop() override {
		mCountDown.doCounting();
	}
};