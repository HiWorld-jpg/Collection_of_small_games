#pragma once
#include "Button.h"

class NumberPad{
private:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	COLORREF mBkColor;
	Button* mButton[9];
public:
	NumberPad(int x, int y, int width, int height, COLORREF bkColor) {
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
		mBkColor = bkColor;
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
					buttonWidth, buttonHeight, numStr, mBkColor, buttonIndex);
			}
		}
	}

	void draw() {
		for (int i = 0; i < 9; i++) {
			mButton[i]->draw();
		}
	}

	bool checkMouseIn(int mouseX, int mouseY) {

	}

	~NumberPad() {
		for (int i = 0; i < 9; i++) {
			delete mButton[i];
		}
	}
};