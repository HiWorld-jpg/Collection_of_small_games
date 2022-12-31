// 20221103
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Frame.h"
#include "Frame_0_menu.h"
#include "Frame_1_jingziqi.h"
#include "Frame_2_xiaoxiaole.h"
#include "TextLabel.h"
#include "Frame_3_shudu.h"

int main() {
	COLORREF bkColor = LIGHTGRAY;
	int bkWidth = 1000;
	int bkHeight = 800;
	initgraph(bkWidth, bkHeight);

	// 创建Button
	Button button_0_jingziqi(100, 100, 130, 50, (char*)"井字棋", bkColor, 0);
	Button button_1_frame_1_to_frame_0(10, 10, 170, 50, (char*)"回到菜单", bkColor, 1);
	Button button_2_frame_1_playAgain(190, 10, 170, 50, (char*)"重玩", bkColor, 2);
	Button button_3_frame_1_loadPlayer1Pic(370, 10, 190, 50, (char*)"玩家1图片", bkColor, 3);
	Button button_4_frame_1_loadPlayer2Pic(570, 10, 190, 50, (char*)"玩家2图片", bkColor, 4);
	Button button_5_frame_1_ResetDefaultPic(770, 10, 170, 50, (char*)"重置图片", bkColor, 5);
	Button button_6_xiaoxiaole(100, 160, 130, 50, (char*)"消消乐", bkColor, 6);
	Button button_7_frame_2_to_frame_0(10, 10, 170, 50, (char*)"回到菜单", bkColor, 7);
	Button button_8_frame_2_playAgain(190, 10, 170, 50, (char*)"重玩", bkColor, 8);
	Button button_9_frame_shudu(100, 220, 130, 50, (char*)"数独", bkColor, 9);
	Button button_10_frame_3_to_frame_0(10, 10, 170, 50, (char*)"回到菜单", bkColor, 10);
	Button button_11_frame_3_load_file(190, 10, 100, 50, (char*)"载入", bkColor, 11);
	Button button_12_frame_3_save_data(300, 10, 100, 50, (char*)"储存", bkColor, 12);
	NumberPad* numberPad_1_frame_3 = new NumberPad(780, 160, 200, 150, bkColor, 13); // 13到13+8分别表示数字1到9
	Button button_22_frame_3_clearBlock(780, 320, 100, 50, (char*)"擦除", bkColor, 22);
	Button button_23_frame_3_clearBoard(890, 320, 100, 50, (char*)"重置", bkColor, 23);
	Button button_24_frame_3_checkBoard(780, 380, 100, 50, (char*)"检查", bkColor, 24);
	Button button_25_frame_3_changeMode(410, 10, 170, 50, (char*)"解题模式", bkColor, 25);
	Button button_26_frame_3_maxSolve(780, 500, 210, 50, (char*)"最大值解法", bkColor, 26);
	Button button_27_frame_3_minSolve(780, 560, 210, 50, (char*)"最小值解法", bkColor, 27);
	Button button_28_frame_3_animation(590, 10, 170, 50, (char*)"动画: 开", bkColor, 28);

	// 创建Frame
	Frame* currFrame = nullptr;
	Frame_0_menu frame_0_menu(bkWidth, bkHeight, bkColor, 0);
	Frame_1_jingziqi frame_1_jingziqi(bkWidth, bkHeight, bkColor, 1,
		(bkWidth - bkHeight / 2) / 2, bkHeight / 4, bkHeight / 2, bkHeight / 2, bkColor, BLACK);
	Frame_2_xiaoxiaole frame_2_xiaoxiaole(bkWidth, bkHeight, bkColor, 2,
		200, 180, 600, 600, bkColor, BLACK);
	Frame_3_shudu frame_3_shudu(bkWidth, bkHeight, bkColor, 3,
		200, 160, 540, 540, bkColor, BLACK, numberPad_1_frame_3);

	// 注册Button, 顺序要严格按照Button index添加
	/*
	Button* allButtons[100];
	allButtons[0] = &button_0_jingziqi;
	allButtons[1] = &button_1_frame_1_to_frame_0;
	allButtons[2] = &button_2_frame_1_playAgain;
	allButtons[3] = &button_3_frame_1_loadPlayer1Pic;
	allButtons[4] = &button_4_frame_1_loadPlayer2Pic;
	allButtons[5] = &button_5_frame_1_ResetDefaultPic;
	allButtons[6] = &button_6_xiaoxiaole;
	allButtons[7] = &button_7_frame_2_to_frame_0;
	allButtons[8] = &button_8_frame_2_playAgain;
	allButtons[9] = &button_9_frame_shudu;
	allButtons[10] = &button_10_frame_3_to_frame_0;
	allButtons[11] = &button_11_frame_3_load_file;
	allButtons[12] = &button_12_frame_3_save_data;
	*/

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
	TextLabel text_8_frame_2(350, 70, 300, 100, bkColor, BLACK, (char*)"消消乐", true, 8);
	TextLabel text_9_frame_2(20, 180, 150, 50, bkColor, BLACK, (char*)"刚才消除", true, 9);
	TextLabel text_10_frame_2(60, 230, 30, 60, bkColor, BLACK, (char*)"0", true, 10);
	TextLabel text_11_frame_3(890, 380, 100, 50, BLUE, WHITE, (char*)"无冲突", true, 11);
	TextLabel text_12_frame_3(890, 380, 100, 50, GREEN, WHITE, (char*)"已完成", true, 12);
	TextLabel text_13_frame_3(890, 380, 100, 50, RED, WHITE, (char*)"有冲突", true, 13);
	TextLabel text_14_frame_3(780, 440, 210, 50, CYAN, BLACK, (char*)"耗时0分0秒0毫秒", true, 14);

	// 将Button加入所属的Frame
	frame_0_menu.addButton(&button_0_jingziqi);
	frame_0_menu.addButton(&button_6_xiaoxiaole);
	frame_0_menu.addButton(&button_9_frame_shudu);

	frame_1_jingziqi.addButton(&button_1_frame_1_to_frame_0);
	frame_1_jingziqi.addButton(&button_2_frame_1_playAgain);
	frame_1_jingziqi.addButton(&button_3_frame_1_loadPlayer1Pic);
	frame_1_jingziqi.addButton(&button_4_frame_1_loadPlayer2Pic);
	frame_1_jingziqi.addButton(&button_5_frame_1_ResetDefaultPic);

	frame_2_xiaoxiaole.addButton(&button_7_frame_2_to_frame_0);
	frame_2_xiaoxiaole.addButton(&button_8_frame_2_playAgain);

	frame_3_shudu.addButton(&button_10_frame_3_to_frame_0);
	frame_3_shudu.addButton(&button_11_frame_3_load_file);
	frame_3_shudu.addButton(&button_12_frame_3_save_data);
	frame_3_shudu.addButton(numberPad_1_frame_3);
	frame_3_shudu.addButton(&button_22_frame_3_clearBlock);
	frame_3_shudu.addButton(&button_23_frame_3_clearBoard);
	frame_3_shudu.addButton(&button_24_frame_3_checkBoard);
	frame_3_shudu.addButton(&button_25_frame_3_changeMode);
	frame_3_shudu.addButton(&button_26_frame_3_maxSolve);
	frame_3_shudu.addButton(&button_27_frame_3_minSolve);
	frame_3_shudu.addButton(&button_28_frame_3_animation);

	frame_1_jingziqi.addText(&text_0_frame_1);
	frame_1_jingziqi.addText(&text_1_frame_1);
	frame_1_jingziqi.addText(&text_2_frame_1);
	frame_1_jingziqi.addText(&text_3_frame_1);
	frame_1_jingziqi.addText(&text_4_frame_1);
	frame_1_jingziqi.addText(&text_5_frame_1);
	frame_1_jingziqi.addText(&text_6_frame_1);
	frame_1_jingziqi.addText(&text_7_frame_1);

	frame_2_xiaoxiaole.addText(&text_8_frame_2);
	frame_2_xiaoxiaole.addText(&text_9_frame_2);
	frame_2_xiaoxiaole.addText(&text_10_frame_2);

	frame_3_shudu.addText(&text_11_frame_3);
	frame_3_shudu.addText(&text_12_frame_3);
	frame_3_shudu.addText(&text_13_frame_3);
	frame_3_shudu.addText(&text_14_frame_3);

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
				SetWindowText(GetHWnd(), "小游戏合集--井字棋");
			} else if (currClickedButtonIndex == 1) {
				currFrame = &frame_0_menu;
				currFrame->init();
				SetWindowText(GetHWnd(), "小游戏合集--菜单");
			} else if (currClickedButtonIndex == 6) {
				currFrame = &frame_2_xiaoxiaole;
				currFrame->init();
				SetWindowText(GetHWnd(), "小游戏合集--消消乐");
			} else if (currClickedButtonIndex == 7) {
				currFrame = &frame_0_menu;
				currFrame->init();
				SetWindowText(GetHWnd(), "小游戏合集--菜单");
			} else if (currClickedButtonIndex == 9) {
				currFrame = &frame_3_shudu;
				currFrame->init();
				SetWindowText(GetHWnd(), "小游戏合集--数独");
			} else if (currClickedButtonIndex == 10) {
				currFrame = &frame_0_menu;
				currFrame->init();
				SetWindowText(GetHWnd(), "小游戏合集--菜单");
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
