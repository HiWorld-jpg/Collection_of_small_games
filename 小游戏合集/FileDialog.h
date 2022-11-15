#pragma once
#include <windows.h>
#include <Commdlg.h>
#include <stdio.h>
#include <ShlObj.h>
#include <graphics.h>

class MyDialog {
public:
	static const int PATH_MAX_LENGTH = 300;
	static const int FILE_NAME_MAX_LENGTH = 300;

	/*
	* 入参path同时也是出参，最大字符长度300
	* 返回值 : 成功 1, 失败 0
	*/
	static int selectFileDialog(char* path) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrTitle = "选择一个文件";
		ofn.lpstrFile = path;
		ofn.nMaxFile = PATH_MAX_LENGTH;
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		return GetOpenFileName(&ofn);
	}

	static int selectFileDialogBySuffix(char *suffix, char* path) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrTitle = "选择一个文件";
		ofn.lpstrFile = path;
		ofn.nMaxFile = PATH_MAX_LENGTH;
		ofn.lpstrFilter = suffix;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		return GetOpenFileName(&ofn);
	}

	static int selectDirDialog(char *path) {
		BROWSEINFO bInfo = { 0 };
		bInfo.lpszTitle = "选择一个文件夹";
		bInfo.hwndOwner = GetForegroundWindow();
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON;
		LPITEMIDLIST lpDlist = NULL;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL) {
			SHGetPathFromIDList(lpDlist, path);
			return 1;
		}
		return 0;
	}

	static int inputDialog(char* result) {
		const char* title = "输入文件名";
		if (InputBox(result, FILE_NAME_MAX_LENGTH, NULL, title, NULL, 0, 0, false)) {
			return 1;
		}
		return 0;
	}

};
