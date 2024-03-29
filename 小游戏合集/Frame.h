#pragma once
#include<graphics.h>
#include"Button.h"
#include"TextLabel.h"

#define MAX_BUTTON_NUM 100
#define MAX_TEXT_NUM 100

class Frame {
protected:
	int mFrameWidth;
	int mFrameHeight;
	COLORREF mFrameBkColor;
	int mGlobalIndex;
	Button* mButtonList[MAX_BUTTON_NUM];
	int mButtonNum;
	TextLabel* mTextList[MAX_TEXT_NUM];
	int mTextNum;

public:
	Frame(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) {
		mFrameWidth = frameWidth;
		mFrameHeight = frameHeight;
		mFrameBkColor = frameBkColor;
		mGlobalIndex = globalIndex;
		mButtonNum = 0;
		mTextNum = 0;
	}

	virtual int retClickedButton(int mouseX, int mouseY) const {
		for (int i = 0; i < mButtonNum; i++) {
			if (mButtonList[i]->checkMouseIn(mouseX, mouseY) == true) {
				return mButtonList[i]->getGlobalIndex();
			}
		}
		return -1;
	}

	virtual Button* getButtonByEventIndex(int eventIndex) const {
		for (int i = 0; i < mButtonNum; i++) {
			if (mButtonList[i]->getGlobalIndex() == eventIndex) {
				return mButtonList[i];
			}
		}
		return nullptr;
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

	virtual bool addText(TextLabel* currText) {
		if (currText == nullptr) {
			return false;
		}
		if (mTextNum == MAX_TEXT_NUM) {
			return false;
		}
		mTextList[mTextNum] = currText;
		mTextNum++;
		return true;
	}

	virtual void init() {
		closegraph();
		initgraph(mFrameWidth, mFrameHeight);
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
		for (int i = 0; i < mTextNum; i++) {
			mTextList[i]->draw();
		}
	}

	virtual int processMouseClickDown(int mouseX, int mouseY) {
		int clickedMouseIndex = -1;
		for (int i = 0; i < mButtonNum; i++) {
			if (mButtonList[i]->checkMouseIn(mouseX, mouseY) == true) {
				mButtonList[i]->drawButtonDown(mouseX, mouseY);
				clickedMouseIndex = mButtonList[i]->getGlobalIndex();
				break;
			}
		}
		return clickedMouseIndex;
	}

	virtual int processMouseClickUp(int mouseX, int mouseY) {
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

	virtual void processMouseDownMove(int mouseX, int mouseY) {

	}

	virtual void processKeyBoardUp(unsigned char vkcode) {

	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) {}

	virtual void processSomethingInLoop() {}
};