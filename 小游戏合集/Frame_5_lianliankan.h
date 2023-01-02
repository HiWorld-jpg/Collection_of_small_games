#pragma once
#include <pthread.h>
#include "Frame.h"

class Frame_5_lianliankan : public Frame {
private:
	static void* testFunc1(void* arg) {  // 如果不加static，函数传给pthread_create会提示函数参数类型不兼容
		double loopMax = 200000;
		for (int i = 0; i < loopMax; i++) {
			circle(400, 400, i / loopMax * 400);
		}
		return nullptr;
	}

	static void* testFunc2(void* arg) {
		double loopMax = 200000;
		for (int i = 0; i < loopMax; i++) {
			circle(400, 400, i / loopMax * 400);
		}
		return nullptr;
	}

public:
	Frame_5_lianliankan(int frameWidth, int frameHeight, COLORREF frameBkColor, int globalIndex) :
		Frame(frameWidth, frameHeight, frameBkColor, globalIndex) {
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

	virtual void processEvent(int eventIndex, int mouseX, int mouseY) override {
		if (eventIndex == 36) {
			//testFunc1(nullptr);
			pthread_t pt; //定义线程ID
			pthread_create(&pt, NULL, testFunc1, NULL); //创建线程
			//pthread_join(pt, NULL); //等待线程结束
		} else if (eventIndex == 37) {
			// testFunc2(nullptr);
			pthread_t pt; //定义线程ID
			pthread_create(&pt, NULL, testFunc2, NULL); //创建线程
			//pthread_join(pt, NULL); //等待线程结束
		}
	}
};