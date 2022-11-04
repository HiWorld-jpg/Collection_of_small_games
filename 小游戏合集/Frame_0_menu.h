#pragma once
#include "Frame.h"

class Frame_0_menu : public Frame {
public:
	Frame_0_menu(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) : 
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) { }
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
};