#pragma once
#include "Common_Define.h"

//ȭ���� �׸��µ� �ʿ��� �Լ����� ������ ���� ���̴�.
//��ü �ڷ����� ���� �ʴ´�.
class DrawScreen
{
public:
	DrawScreen() {
		hide_cursor();
		system("cls");
	}
	void draw_layout(const ICON_NUMBER arr[][SIZE_OF_COLUMN_SCREEN], const int row);
	void center_text(string text[], int NumberOfStr, bool Line_Number=true, int Sort_Type=0);//����� ���ڸ� ������ִ� �Լ�
	//0�� ��� ����, -1�� ���� ����, +1�� ������ ����
	//LineNumber�� ���� ��ȣ�� ǥ������ ����
	void update_info(int player_score, int ai_score, int gen);
	//player_score�̳� ai_score�� -1�� ǥ�� ���ϱ�
private:
	void hide_cursor();
	void draw_convert(ICON_NUMBER array_icon_symbol);//�迭�� �ִ� SYMBOL�� ICON���� ������ִ� �Լ�
	void gotoxy(int x, int y);//ICON�� ����Ҷ� ����ϴ� �̵� �Լ� CommonDefine�� USE_2BYTE_CHAR ���
	void gotoxy_for_text(int x, int y);//���ڰ� 2����Ʈ �ΰ��� ������� ���� �Լ�
	void gotoxy_for_only_1byte(int x, int y);
	void gotoxy_for_only_2byte(int x, int y);
};








