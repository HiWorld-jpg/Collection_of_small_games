#pragma once
#include "Button.h"

class NumberPad : public Button {
private:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	COLORREF mBkColor;
	Button* mButton[9];
	int mGlobalIndex;
public:
	NumberPad(int x, int y, int width, int height, COLORREF bkColor, int globalIndex) : 
		Button(x, y, width, height, NULL, bkColor, globalIndex) {
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
		mBkColor = bkColor;
		mGlobalIndex = globalIndex;
		createButton();
	}

	void createButton() {
		int gap = 2;
		int buttonWidth = mWidth / 3 - gap;
		int buttonHeight = mHeight / 3 - gap;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int buttonX = mX + j * (buttonWidth + gap);
				int buttonY = mY + i * (buttonHeight + gap);
				char numStr[2] = { 0 };
				int buttonIndex = i * 3 + j;
				numStr[0] = '1' + buttonIndex;
				mButton[buttonIndex] = new Button(buttonX, buttonY, 
					buttonWidth, buttonHeight, numStr, mBkColor, buttonIndex + mGlobalIndex);
			}
		}
	}

	virtual void draw() const override {
		for (int i = 0; i < 9; i++) {
			mButton[i]->draw();
		}
	}
	
	virtual void drawButtonDown(int mouseX, int mouseY) const {
		for (int i = 0; i < 9; i++) {
			if (mButton[i]->checkMouseIn(mouseX, mouseY)) {
				mButton[i]->drawButtonDown(mouseX, mouseY);
				break;
			}
		}
	}

	virtual void drawButtonUp(int mouseX, int mouseY) const {
		for (int i = 0; i < 9; i++) {
			if (mButton[i]->checkMouseIn(mouseX, mouseY)) {
				mButton[i]->drawButtonUp(mouseX, mouseY);
				break;
			}
		}
	}

	int getPressedNumber(int mouseX, int mouseY) const {
		for (int i = 0; i < 9; i++) {
			if (mButton[i]->checkMouseIn(mouseX, mouseY)) {
				return i + 1;
			}
		}
		return -1; // 没有按
	}

	~NumberPad() {
		for (int i = 0; i < 9; i++) {
			delete mButton[i];
		}
	}
};