#pragma once
#include "Frame.h"
#include "TimeCount.h"

typedef struct PicType {
	int id;
	char *path;
} PicType;

class Frame_5_lianliankan : public Frame {
private:
	int mBoardX;
	int mBoardY;
	int mBoardWidth;
	int mBoardHeight;
	COLORREF mBoardBkColor;
	COLORREF mBoardOutlineColor;
	CountDown mCountDown;
	static const int mPicMaxNum = 100;
	PicType mPic[mPicMaxNum]; // 只给0至mPicTypeNum-1的下标元素赋值，因此取元素时需要严格使用mPicTypeNum约束
	int mPicTypeNum;
	static const int mBoardXMax = 20;
	static const int mBoardYMax = 15;
	PicType* mBoardData[mBoardXNum][mBoardYNum];  // 大小固定为横向20个格子，纵向15个格子
	bool mBoardDataPatten[mBoardXNum][mBoardYNum];  // 限制格子能不能加元素，true表示会填充元素，false表示不填充即当作空白处理

public:
	Frame_5_lianliankan(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex,
		int boardX, int boardY, int boardWidth, int boardHeight, COLORREF boardBkColor, COLORREF boardOutlineColor) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex),
		mCountDown(100, 100, 200, 80, frameBkColor, BLACK, 20) {

		mBoardX = boardX;
		mBoardY = boardY;
		mBoardWidth = boardWidth;
		mBoardHeight = boardHeight;
		mBoardBkColor = boardBkColor;
		mBoardOutlineColor = boardOutlineColor;

		initPic();

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
	}

	void initPic() {
		mPic[0].path = (char*)"./picture/lianliankan_picture/axial_symmetry.png";
		mPic[1].path = (char*)"./picture/lianliankan_picture/black_ball.png";

		mPicTypeNum = 2;   //注意不要超过mPicMaxNum
		for (int i = 0; i < mPicTypeNum; i++) {
			mPic[i].id = i;
		}
	}

	void initBoardDataPatten() {
		for (int i = 0; i < mBoardXNum; i++) {
			for (int j = 0; j < mBoardYNum; j++) {
				mBoardDataPatten[i][j] = true;
			}
		}
		// 后续实现读取文件来切换模式
	}

	void initBoardData() {
		initBoardDataPatten();
		for (int i = 0; i < mBoardXNum; i++) {
			for (int j = 0; j < mBoardYNum; j++) {

			}
		}

	}

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == 36) {
			mCountDown.startCounting();
		} else if (eventIndex == 37) {

		}
	}

	virtual void processSomethingInLoop() override {
		mCountDown.doCounting();
	}
};