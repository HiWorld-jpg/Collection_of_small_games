#pragma once
#include "Frame.h"

class GuaGuaCard {
private:
	IMAGE mBackImage;      // 初始背景图，不绘制刮刮区的图片
	IMAGE mGuaFrontImage;  // 刮刮区初始时候的遮挡图片
	IMAGE mGuaBackImage;   // 刮刮区实际的底层图片
	IMAGE mGuaMaskImage;   // 刮刮区的掩码图

	int value;  // 刮刮区的值

	int mBackImageX;
	int mBackImageY;
	int mBackImageWidth;
	int mBackImageHeight;
	// 刮刮区的位置和大小由初始背景图的位置决定
	int mGuaGuaImageX;
	int mGuaGuaImageY;
	int mGuaGuaImageWidth;
	int mGuaGuaImageHeight;

	int mMouseX;
	int mMouseY;

public:
	GuaGuaCard() {}

	void initGuaGuaCard(int backImageX, int backImageY, int backImageWidth, int backImageHeight) {
		mBackImageX = backImageX;
		mBackImageY = backImageY;
		mBackImageWidth = backImageWidth;
		mBackImageHeight = backImageHeight;
		mGuaGuaImageWidth = mBackImageWidth * 0.8;
		mGuaGuaImageHeight = mBackImageHeight * 0.3;
		mGuaGuaImageX = mBackImageX + backImageWidth * 0.1;
		mGuaGuaImageY = mBackImageY + mBackImageHeight * 0.9 - mGuaGuaImageHeight;

		mBackImage.Resize(mBackImageWidth, mBackImageHeight);
		mGuaFrontImage.Resize(mGuaGuaImageWidth, mGuaGuaImageHeight);
		mGuaBackImage.Resize(mGuaGuaImageWidth, mGuaGuaImageHeight);
		mGuaMaskImage.Resize(mGuaGuaImageWidth, mGuaGuaImageHeight);

		initAndDraw();
	}

	void initMousePos() {
		mMouseX = 0;
		mMouseY = 0;
	}

	void updateMousePos(int mouseX, int mouseY) {
		mMouseX = mouseX;
		mMouseY = mouseY;
	}

	void initAndDraw() {
		initMousePos();
		initBackImage();
		initGuaBackImage();
		initGuaFrontImage();
		initGuaMaskImage();
		draw();
	}

	void initBackImage() {
		SetWorkingImage(&mBackImage);
		setfillcolor(RED);
		solidrectangle(0, 0, mBackImage.getwidth(), mBackImage.getheight());
		FunctionUtils::printStrToRectangleArea(mBackImage.getwidth() * 0.2, mBackImage.getheight() * 0.1,
			mBackImage.getwidth() * 0.6, mBackImage.getheight() * 0.2, (char*)"刮刮乐", BLACK);
		SetWorkingImage();
	}

	void initGuaBackImage() {
		SetWorkingImage(&mGuaBackImage);
		setfillcolor(YELLOW);
		solidrectangle(0, 0, mGuaBackImage.getwidth(), mGuaBackImage.getheight());
		FunctionUtils::printStrToRectangleArea(mGuaBackImage.getwidth() * 0.1, mGuaBackImage.getheight() * 0.1,
			mGuaBackImage.getwidth() * 0.8, mGuaBackImage.getheight() * 0.8, (char*)"恭喜发财", BLUE);
		SetWorkingImage();
	}

	void initGuaFrontImage() {
		SetWorkingImage(&mGuaFrontImage);
		setfillcolor(LIGHTGRAY);
		solidrectangle(0, 0, mGuaFrontImage.getwidth(), mGuaFrontImage.getheight());
		SetWorkingImage();
	}

	void initGuaMaskImage() {
		SetWorkingImage(&mGuaMaskImage);
		setfillcolor(WHITE);
		solidrectangle(0, 0, mGuaMaskImage.getwidth(), mGuaMaskImage.getheight());
		SetWorkingImage();
	}

	void draw() const {
		putimage(mBackImageX, mBackImageY, &mBackImage);
		IMAGE temp1 = mGuaFrontImage;
		SetWorkingImage(&temp1);
		putimage(0, 0, &mGuaMaskImage, SRCAND);
		IMAGE temp2 = mGuaMaskImage;
		SetWorkingImage(&temp2);
		putimage(0, 0, &mGuaBackImage, SRCERASE);
		SetWorkingImage(&temp1);
		putimage(0, 0, &temp2, SRCPAINT);
		SetWorkingImage();
		putimage(mGuaGuaImageX, mGuaGuaImageY, &temp1);
	}

	bool checkMouseInGuaArea(int mouseX, int mouseY) {
		if (mouseX >= mGuaGuaImageX && mouseX <= mGuaGuaImageX + mGuaGuaImageWidth &&
			mouseY >= mGuaGuaImageY && mouseY <= mGuaGuaImageY + mGuaGuaImageHeight) {
			return true;
		}
		return false;
	}

	void gua(int mouseX, int mouseY) {
		if (mMouseX == 0 && mMouseY == 0) {
			updateMousePos(mouseX, mouseY);
			return;
		}
		SetWorkingImage(&mGuaMaskImage);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 30);
		line(mouseX - mGuaGuaImageX, mouseY - mGuaGuaImageY, mMouseX - mGuaGuaImageX, mMouseY - mGuaGuaImageY);
		SetWorkingImage();
		draw();
	}

};

class Frame_6_guaguale : public Frame {
private:
	GuaGuaCard guaguaCard;
public:
	Frame_6_guaguale(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex):
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
		guaguaCard.initGuaGuaCard(frameWidth * 0.3, frameHeight * 0.2, frameWidth * 0.4, frameHeight * 0.6);
	}

	virtual void init() override {
		setbkcolor(mFrameBkColor);
		cleardevice();
		for (int i = 0; i < mButtonNum; i++) {
			mButtonList[i]->draw();
		}
		for (int i = 0; i < mTextNum; i++) {
			mTextList[i]->draw();
		}
		guaguaCard.draw();
	}

	virtual void processMouseDownMove(int mouseX, int mouseY) override {
		BeginBatchDraw();
		if (guaguaCard.checkMouseInGuaArea(mouseX, mouseY) == true) {
			guaguaCard.gua(mouseX, mouseY);
		}
		guaguaCard.updateMousePos(mouseX, mouseY);
		FlushBatchDraw();
		EndBatchDraw();
	}

	// 重载这个函数的目的是为了让鼠标弹起那一刻初始化guaguaCard的鼠标数据
	virtual int processMouseClickUp(int mouseX, int mouseY) {
		guaguaCard.initMousePos();

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

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		BeginBatchDraw();

		if (eventIndex == 41) {
			guaguaCard.initAndDraw();
		}
		FlushBatchDraw();
		EndBatchDraw();
	}
};