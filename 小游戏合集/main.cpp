// 20221103
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Frame.h"
#include "Frame_0_menu.h"
#include "Frame_1_jingziqi.h"
#include "TextLabel.h"

int main() {

	COLORREF bkColor = LIGHTGRAY;
	int bkWidth = 1000;
	int bkHeight = 800;
	initgraph(bkWidth, bkHeight);

	// 创建Frame
	Frame* currFrame = nullptr;
	Frame_0_menu frame_0_menu(bkWidth, bkHeight, bkColor, 0);
	Frame_1_jingziqi frame_1_jingziqi(bkWidth, bkHeight, bkColor, 1,
		(bkWidth - bkHeight / 2) / 2, bkHeight / 4, bkHeight / 2, bkHeight / 2, bkColor, BLACK);

	// 创建Button
	Button button_0_jingziqi(100, 100, 130, 50, (char*)"井字棋", bkColor, 0);
	Button button_1_frame_1_to_frame_0(10, 10, 170, 50, (char*)"回到菜单", bkColor, 1);
	Button button_2_frame_1_playAgain(190, 10, 170, 50, (char*)"重玩", bkColor, 2);
	Button button_3_frame_1_loadPlayer1Pic(370, 10, 190, 50, (char*)"玩家1图片", bkColor, 3);
	Button button_4_frame_1_loadPlayer2Pic(570, 10, 190, 50, (char*)"玩家2图片", bkColor, 4);
	Button button_5_frame_1_ResetDefaultPic(770, 10, 170, 50, (char*)"重置图片", bkColor, 5);

	// 注册Button, 顺序要严格按照Button index添加
	Button* allButtons[100];
	allButtons[0] = &button_0_jingziqi;
	allButtons[1] = &button_1_frame_1_to_frame_0;
	allButtons[2] = &button_2_frame_1_playAgain;
	allButtons[3] = &button_3_frame_1_loadPlayer1Pic;
	allButtons[4] = &button_4_frame_1_loadPlayer2Pic;
	allButtons[5] = &button_5_frame_1_ResetDefaultPic;

	int frame1TextX = (bkWidth - bkHeight / 2) / 2;
	int frame1TextY = bkHeight / 4 * 3;
	int frame1TextWidth = bkHeight / 2;
	int frame1TextHeight = bkHeight / 4 / 3;
	TextLabel text_0_frame_1(frame1TextX, frame1TextY, frame1TextWidth, frame1TextHeight, bkColor, GREEN, (char*)"现在轮到玩家1落子", true, 0);
	TextLabel text_1_frame_1(frame1TextX, frame1TextY, frame1TextWidth, frame1TextHeight, bkColor, RED,   (char*)"现在轮到玩家2落子", true, 1);
	TextLabel text_2_frame_1(frame1TextX, frame1TextY, frame1TextWidth, frame1TextHeight, bkColor, BLUE,  (char*)"游戏结束，玩家1赢了", true, 2);
	TextLabel text_3_frame_1(frame1TextX, frame1TextY, frame1TextWidth, frame1TextHeight, bkColor, BLUE,  (char*)"游戏结束，平局", true, 3);
	TextLabel text_4_frame_1(frame1TextX, frame1TextY, frame1TextWidth, frame1TextHeight, bkColor, BLUE,  (char*)"游戏结束，玩家2赢了", true, 4);
	TextLabel text_5_frame_1(frame1TextX, frame1TextY + frame1TextHeight, frame1TextWidth, frame1TextHeight * 0.8, bkColor, BLACK, (char*)"玩家1赢 : 平局 : 玩家2赢", true, 5);
	TextLabel text_6_frame_1(frame1TextX, frame1TextY + frame1TextHeight * 1.6, frame1TextWidth, frame1TextHeight * 1.6, bkColor, BLACK, (char*)"0:0:0", true, 6);
	TextLabel text_7_frame_1(frame1TextX, bkHeight / 4 - frame1TextHeight * 2, frame1TextWidth, frame1TextHeight * 2, bkColor, BLACK, (char*)"井字棋", true, 7);

	// 将Button加入所属的Frame
	frame_0_menu.addButton(&button_0_jingziqi);
	frame_1_jingziqi.addButton(&button_1_frame_1_to_frame_0);
	frame_1_jingziqi.addButton(&button_2_frame_1_playAgain);
	frame_1_jingziqi.addButton(&button_3_frame_1_loadPlayer1Pic);
	frame_1_jingziqi.addButton(&button_4_frame_1_loadPlayer2Pic);
	frame_1_jingziqi.addButton(&button_5_frame_1_ResetDefaultPic);

	frame_1_jingziqi.addText(&text_0_frame_1);
	frame_1_jingziqi.addText(&text_1_frame_1);
	frame_1_jingziqi.addText(&text_2_frame_1);
	frame_1_jingziqi.addText(&text_3_frame_1);
	frame_1_jingziqi.addText(&text_4_frame_1);
	frame_1_jingziqi.addText(&text_5_frame_1);
	frame_1_jingziqi.addText(&text_6_frame_1);
	frame_1_jingziqi.addText(&text_7_frame_1);

	// 初始化第一个窗口
	currFrame = &frame_0_menu;
	currFrame->init();

	ExMessage msg;
	bool mouseClickFlag = false;
	bool keyBoardPressFlag = false;

	while (true) {
		peekmessage(&msg);
		if (msg.vkcode == VK_ESCAPE) {
			break;
		}
		if (msg.lbutton == true && mouseClickFlag == false) {
			mouseClickFlag = true;
			currFrame->processMouseClickDown(msg.x, msg.y);
		}
		if (msg.lbutton == false && mouseClickFlag == true) {
			mouseClickFlag = false;
			int currClickedButtonIndex = currFrame->processMouseClickUp(msg.x, msg.y);
			if (currClickedButtonIndex == 0) {
				currFrame = &frame_1_jingziqi;
				currFrame->init();
			} else if (currClickedButtonIndex == 1) {
				currFrame = &frame_0_menu;
				currFrame->init();
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
