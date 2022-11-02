#pragma once
#include "Frame.h"

class Frame_1_jingziqi : public Frame {
public:
	Frame_1_jingziqi(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
	}
	virtual void init() const override {
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
	}
};