#pragma once
#include "Common_Define.h"

//ȭ���� �׸��µ� �ʿ��� �Լ����� ������ ���� ���̴�.
//��ü �ڷ����� ���� �ʴ´�.
class DrawScreen
{
public:
	DrawScreen() {
		hide_cursor();
		screen_clear();
		prv_row = 0;
		prv_col = 0;
		prv_arr = nullptr;
	}
	void screen_clear();
	void draw_layout(const ICON_NUMBER arr[][SIZE_OF_COL_SCREEN], const size_t row);
	void center_text(const string text[], size_t NumberOfStr, bool Line_Number=true, int Sort_Type=0);//����� ���ڸ� ������ִ� �Լ�
	//0�� ��� ����, -1�� ���� ����, +1�� ������ ����
	//LineNumber�� ���� ��ȣ�� ǥ������ ����
	void update_info(bool LearnMode, int GameTries, int LearnMode_AI_maxscore = -1,
		int CompeteMode_RightAIScore = -1, int CompeteMode_LeftPlayerScore = -1, int Gen = -1, bool Acceleration = false, bool Hide = false);
	//player_score�̳� ai_score�� -1�� ǥ�� ���ϱ�
	static void set_title(string value);
	static void hide_scrollbar();
	static void set_console_size(size_t rows, size_t cols);
private:
	void hide_cursor();
	void draw_convert(ICON_NUMBER array_icon_symbol);//�迭�� �ִ� SYMBOL�� ICON���� ������ִ� �Լ�
	void gotoxy(int x, int y);//ICON�� ����Ҷ� ����ϴ� �̵� �Լ� CommonDefine�� USE_2BYTE_CHAR ���
	void gotoxy_for_only_1byte(int x, int y);
	void gotoxy_for_only_2byte(int x, int y);


	size_t prv_row;
	size_t prv_col;
	int** prv_arr;
};








