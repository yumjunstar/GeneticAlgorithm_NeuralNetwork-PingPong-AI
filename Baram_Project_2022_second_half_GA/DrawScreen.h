#pragma once
#include "Common_Define.h"

//화면을 그리는데 필요한 함수등을 정리해 놓은 곳이다.
//자체 자료형은 갖지 않는다.
class DrawScreen
{
public:
	DrawScreen() {
		hide_cursor();
		screen_clear();
	}
	void screen_clear();
	void draw_layout(const ICON_NUMBER arr[][SIZE_OF_COL_SCREEN], const int row);
	void center_text(string text[], int NumberOfStr, bool Line_Number=true, int Sort_Type=0);//가운데에 글자를 출력해주는 함수
	//0은 가운데 정렬, -1은 왼쪽 정렬, +1은 오른쪽 정렬
	//LineNumber은 라인 번호를 표시할지 여부
	void update_info(bool LearnMode, int GameTries, int LearnMode_AI_maxscore = -1,
		int CompeteMode_RightAIScore = -1, int CompeteMode_LeftPlayerScore = -1, int Gen = -1, bool Acceleration = false, bool Hide = false);
	//player_score이나 ai_score가 -1면 표시 안하기
private:
	void hide_cursor();
	void draw_convert(ICON_NUMBER array_icon_symbol);//배열에 있는 SYMBOL을 ICON으로 출력해주는 함수
	void gotoxy(int x, int y);//ICON을 출력할때 사용하는 이동 함수 CommonDefine에 USE_2BYTE_CHAR 고려
	void gotoxy_for_only_1byte(int x, int y);
	void gotoxy_for_only_2byte(int x, int y);
};








