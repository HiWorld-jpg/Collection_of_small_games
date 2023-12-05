#pragma once
#include<graphics.h>
#include<string.h>

class Button
{
private:
	int x;
	int y;
	int width;
	int height;
	static const int mTextMaxLen = 100;
	char text[mTextMaxLen] = { 0 };
	COLORREF bkColor;
	int globalIndex;
	// 是否开始按钮文字大小自适应
	bool mEnableAutoAdapt = false;
public:
	Button(int x, int y, int width, int height, char* text, COLORREF bkColor, int globalIndex, bool enableAutoAdapt = false) {
		setX(x);
		setY(y);
		setWidth(width);
		setHeight(height);
		setText(text);
		setBkColor(bkColor);
		setGlobalIndex(globalIndex);
		setEnableAutoAdapt(enableAutoAdapt);
	}
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setText(char* text) {
		if (text == NULL) {
			return;
		}
		int textLen = strlen(text);
		if (textLen >= mTextMaxLen) {
			return;
		}
		strncpy(this->text, text, textLen); // strncpy不会向字符串末尾添'\0'
		this->text[textLen] = '\0';
	}
	void setBkColor(COLORREF bkColor) { this->bkColor = bkColor; }
	void setGlobalIndex(int globalIndex) { this->globalIndex = globalIndex; }
	void setEnableAutoAdapt(bool enableAutoAdapt) { mEnableAutoAdapt = enableAutoAdapt; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	char* getText() { return text; }
	int getGlobalIndex() const { return globalIndex; }

	virtual void draw() const {
		drawButtonUp(0, 0); // 此时只画按钮，因此鼠标位置不关注
		drawButtonText();
	}

	virtual void drawButtonUp(int mouseX, int mouseY) const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
		setlinecolor(BLACK);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
		setlinecolor(WHITE);
		line(x, y, x + width, y);
		line(x, y, x, y + height);
	}

	virtual void drawButtonDown(int mouseX, int mouseY) const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
		setlinecolor(BLACK);
		line(x, y, x + width, y);
		line(x, y, x, y + height);
		setlinecolor(WHITE);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
	}

	bool checkChineseChar(const char* text) const {
		int textLen = strlen(text);
		for (int i = 0; i < textLen; i++) {
			// 我不知道为啥是160，网上查到的
			char currChar = text[i];
			unsigned int uiChar = (unsigned int)currChar;
			if (uiChar > 160) {
				return true;
			}
		}
		return false;
	}

	void outputColorText(COLORREF outputColor) const {
		settextcolor(outputColor);
		if (mEnableAutoAdapt == false) {
			int textLen = strlen(text);
			int fontHeight = height * 0.8;
			int fontWidth = fontHeight / 2;
			int spaceLeft = (width - fontWidth * textLen) / 2;
			// 如果高度满足的情况下长度足够
			if (spaceLeft > 0) {
				settextstyle(fontHeight, fontWidth, _T("Fixedsys"));
				outtextxy(x + spaceLeft, y + height * 0.1, text);
			}
		} else {
			int textLen = strlen(text);
			int fontHeight = height * 0.8;
			int fontWidth = width * 1 / textLen;
			settextstyle(fontHeight, fontWidth, _T("宋体"));
			outtextxy(x, y + height * 0.1, text);
		}
	}

	void drawButtonText() const {
		outputColorText(BLACK);
	}

	void clearButtonText() const {
		outputColorText(bkColor);
	}

	virtual bool checkMouseIn(int mouseX, int mouseY) const {
		if (mouseX < x || mouseX > x + width) {
			return false;
		}
		if (mouseY < y || mouseY > y + height) {
			return false;
		}
		return true;
	}
};

