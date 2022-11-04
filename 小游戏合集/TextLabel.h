#pragma once
#include <graphics.h>
#include <string.h>

class TextLabel {
private:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	COLORREF mBackColor;
	COLORREF mFontColor;
	static const int mTextMaxLen = 100;
	char mText[mTextMaxLen];
	bool mVisibility;
	int mGlobalIndex;

public:
	TextLabel(int x, int y, int width, int height, COLORREF backColor, COLORREF fontColor, 
		char* text, bool visibility, int globalIndex) {
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
		mBackColor = backColor;
		mFontColor = fontColor;
		mGlobalIndex = globalIndex;
		mVisibility = visibility;
		setText(text);
	}

	void setVisibility(bool visibility) {
		mVisibility = visibility;
	}

	void setText(char* text) {
		if (text == NULL) {
			return;
		}
		int textLen = strlen(text);
		if (textLen >= mTextMaxLen) {
			return;
		}
		strncpy(mText, text, textLen); // strncpy不会向字符串末尾添'\0'
		mText[textLen] = '\0';
	}

	void clear() const {
		setbkcolor(mBackColor);
		clearrectangle(mX, mY, mX + mWidth, mY + mHeight);
	}

	void draw() const {
		if (mVisibility == false) {
			return;
		}
		clear();
		int textLen = strlen(mText);
		int fontHeight = mHeight * 0.8;
		int fontWidth = mWidth / textLen;

		/* 测试字体格式是否对齐
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
		rectangle(mX, mY, mX + mWidth, mY + mHeight);
		for (int i = 0; i <= textLen; i++) {
			line(mX + i * fontWidth, mY, mX + i * fontWidth, mY + mHeight);
		}
		*/
		// Consolas字体可以完美对齐。
		// 一个中文字符相当于2个英文字符，也就是说strlen("中文")的结果是4
		settextstyle(fontHeight, fontWidth, _T("Consolas"));
		setbkmode(TRANSPARENT);
		settextcolor(mFontColor);
		outtextxy(mX, mY + mHeight * 0.1, mText);
	}
};