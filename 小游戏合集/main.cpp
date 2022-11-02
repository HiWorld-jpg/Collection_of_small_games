// 20221030
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Frame.h"
#include "Frame_0_menu.h"
#include "Frame_1_jingziqi.h"

int main() {

	COLORREF bkColor = LIGHTGRAY;
	int bkWidth = 1000;
	int bkHeight = 800;
	initgraph(bkWidth, bkHeight);

	// 创建Frame
	Frame* currFrame = nullptr;
	Frame_0_menu frame_0_menu(bkWidth, bkHeight, bkColor, 0);
	Frame_1_jingziqi frame_1_jingziqi(bkWidth, bkHeight, bkColor, 1);

	// 创建Button
	Button button_0_jingziqi(100, 100, 130, 50, (char*)"井字棋", bkColor, 0);
	Button button_1_frame_1_to_frame_0(100, 100, 170, 50, (char*)"回到菜单", bkColor, 1);

	// 注册Button, 顺序要严格按照Button index添加
	Button* allButtons[100];
	allButtons[0] = &button_0_jingziqi;
	allButtons[1] = &button_1_frame_1_to_frame_0;

	// 将Button加入所属的Frame
	frame_0_menu.addButton(&button_0_jingziqi);

	frame_1_jingziqi.addButton(&button_1_frame_1_to_frame_0);

	// 初始化第一个窗口
	currFrame = &frame_0_menu;
	currFrame->init();

	ExMessage msg;
	bool mouseClickFlag = false;
	bool keyBoardPressFlag = false;
	Button* currClickedButton = nullptr;

	while (true) {
		peekmessage(&msg);
		if (msg.vkcode == VK_ESCAPE) {
			break;
		}
		if (msg.lbutton == true && mouseClickFlag == false) {
			mouseClickFlag = true;
			int currClickedButtonIndex = currFrame->retClickedButton(msg.x, msg.y);
			if (currClickedButtonIndex != -1) {
				currClickedButton = allButtons[currClickedButtonIndex];
				currClickedButton->drawButtonDown();
			}
		}
		if (msg.lbutton == false && mouseClickFlag == true) {
			mouseClickFlag = false;
			if (currClickedButton != nullptr) {
				currClickedButton->drawButtonUp();
				int currClickedButtonIndex = currClickedButton->getGlobalIndex();
				// 处理Button事件
				if (currClickedButtonIndex == 0) {
					currFrame = &frame_1_jingziqi;
					currFrame->init();
				} else if (currClickedButtonIndex == 1) {
					currFrame = &frame_0_menu;
					currFrame->init();
				}
				currClickedButton = nullptr;
			}
		}

		if (msg.message == WM_KEYDOWN && keyBoardPressFlag == false) {
			keyBoardPressFlag = true;
		}

		if (msg.message == WM_KEYUP && keyBoardPressFlag == true) {
			keyBoardPressFlag = false;
			if (msg.vkcode == VK_ESCAPE) {
				return 0;
			}
		}

	}

	closegraph();
	
	return 0;
}
