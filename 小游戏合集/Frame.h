#pragma once
#include<graphics.h>
#include"Button.h"

#define MAX_BUTTON_NUM 100

class Frame {
protected:
	int mFrameWidth;
	int mFrameHeight;
	COLORREF mFrameBkColor;
	int mGlobalIndex;
	Button* mButtonList[MAX_BUTTON_NUM];
	int mButtonNum;

public:
	Frame(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) {
		mFrameWidth = frameWidth;
		mFrameHeight = frameHeight;
		mFrameBkColor = frameBkColor;
		mGlobalIndex = globalIndex;
		mButtonNum = 0;
	}

	virtual int retClickedButton(int mouseX, int mouseY) const {
		for (int i = 0; i < mButtonNum; i++) {
			if (mButtonList[i]->checkMouseIn(mouseX, mouseY) == true) {
				return mButtonList[i]->getGlobalIndex();
			}
		}
		return -1;
	}

	virtual bool addButton(Button* currButton) {
		if (currButton == nullptr) {
			return false;
		}
		if (mButtonNum == MAX_BUTTON_NUM) {
			return false;
		}
		mButtonList[mButtonNum] = currButton;
		mButtonNum++;
		return true;
	}

	virtual void init() const {
		closegraph();
		initgraph(mFrameWidth, mFrameHeight);
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
	}
};