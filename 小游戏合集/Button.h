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
	char* text;
	COLORREF bkColor;
	int globalIndex;
public:
	Button(int x, int y, int width, int height, char* text, COLORREF bkColor, int globalIndex) {
		setX(x);
		setY(y);
		setWidth(width);
		setHeight(height);
		setText(text);
		setBkColor(bkColor);
		setGlobalIndex(globalIndex);
	}
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setText(char* text) { this->text = text; }
	void setBkColor(COLORREF bkColor) { this->bkColor = bkColor; }
	void setGlobalIndex(int globalIndex) { this->globalIndex = globalIndex; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	char* getText() const { return text; }
	int getGlobalIndex() const { return globalIndex; }

	void draw() const {
		drawButtonUp();
		drawButtonText();
	}

	void drawButtonUp() const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
		setlinecolor(BLACK);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
		setlinecolor(WHITE);
		line(x, y, x + width, y);
		line(x, y, x, y + height);
	}

	void drawButtonDown() const {
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
		setlinecolor(BLACK);
		line(x, y, x + width, y);
		line(x, y, x, y + height);
		setlinecolor(WHITE);
		line(x, y + height, x + width, y + height);
		line(x + width, y, x + width, y + height);
	}

	void drawButtonText() const {
		settextcolor(BLACK);
		int textLen = strlen(text);
		int fontHeight = height * 0.8;
		int fontWidth = fontHeight / 2;
		int spaceLeft = (width - fontWidth * textLen) / 2;
		// 如果高度满足的情况下长度足够
		if (spaceLeft > 0) {
			settextstyle(fontHeight, fontWidth, _T("Fixedsys"));
			outtextxy(x + spaceLeft, y + height * 0.1, text);
		} else {
			// settextstyle(fontHeight, width / textLen, _T("Fixedsys"));
			// outtextxy(x, y + height * 0.1, text);
		}
		
	}

	bool checkMouseIn(int mouseX, int mouseY) const {
		if (mouseX < x || mouseX > x + width) {
			return false;
		}
		if (mouseY < y || mouseY > y + height) {
			return false;
		}
		return true;
	}
};

