#pragma once
#include <graphics.h>
#include <string.h>

class FunctionUtils {
public:
	static void printStrToRectangleArea(int areaX, int areaY, int areaWidth, int areaHeight, char* str, COLORREF fontColor) {
		// 竖向上下各空出10%高度，因此字体长度是80%高度
		// 字体总宽度设置为字体长度的二分之一
		int fontHeight = areaHeight * 0.8;
		int fontWidth = fontHeight / 2;
		int strLength = strlen(str);
		if (strLength == 0) {
			return;
		}
		if (areaWidth < strLength * fontWidth) {
			fontWidth = areaWidth / strLength;
		}
		int fontX = areaX + (areaWidth - strLength * fontWidth) / 2;
		int fontY = areaY + areaHeight * 0.1;

		settextstyle(fontHeight, fontWidth, _T("Consolas"));
		setbkmode(TRANSPARENT);
		settextcolor(fontColor);
		outtextxy(fontX, fontY, str);
	}
};