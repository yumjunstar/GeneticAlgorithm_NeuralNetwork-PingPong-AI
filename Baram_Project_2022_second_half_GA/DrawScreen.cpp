#include "DrawScreen.h"

void DrawScreen::screen_clear()
{
	system("cls");
	prv_row = 0;
	prv_col = 0;
	hide_scrollbar();
}

void DrawScreen::draw_layout(const ICON_NUMBER arr[][SIZE_OF_COL_SCREEN], const size_t row) {
	if ((prv_row != row) || (prv_col != SIZE_OF_COL_SCREEN)) {//아예 크기 조차 다르면 다시 지우고 다시 해야 한다.
		//이전 배열을 저장한 것을 그대로 사용할 수 없고 의미가 없으므로 지운다.
		if (prv_arr != nullptr) {
			for (size_t k = 0; k < prv_row; ++k) {
				delete[] prv_arr[k];
			}
		}

		prv_arr = new int* [row];
		assert(prv_arr);
		for (size_t k = 0; k < row; ++k) {
			prv_arr[k] = new int[SIZE_OF_COL_SCREEN];
			assert(prv_arr[k]);
		}



		system("cls");
		for (size_t i = 0; i < row; ++i) {
			for (size_t j = 0; j < SIZE_OF_COL_SCREEN; ++j) {
				gotoxy(j, i);//x, y;
				draw_convert(arr[i][j]);
			}
		}
	}
	else {//크기가 같으면
		for (size_t i = 0; i < row; ++i) {
			for (size_t j = 0; j < SIZE_OF_COL_SCREEN; ++j) {
				if (prv_arr[i][j] != arr[i][j]) {//바뀐 부분만 이동해서 출력한다.
					gotoxy(j, i);//x, y;
					draw_convert(arr[i][j]);
				}
			}
		}
	}
	//0->0
	//1->2
	//2->4
	//3->6
	//2k 으로 옮기게 해야 한다.


	for (size_t i = 0; i < row; ++i) {//여기는 항상 이전과 이후의 배열 크기가 같아야 한다.
		for (size_t j = 0; j < SIZE_OF_COL_SCREEN; ++j) {
			prv_arr[i][j] = arr[i][j];
		}
	}
	prv_row = row;
	prv_col = SIZE_OF_COL_SCREEN;

	assert(_CrtCheckMemory());
}
void DrawScreen::draw_convert(ICON_NUMBER array_icon_symbol) {
	switch (array_icon_symbol) {
	case BALL_SYMBOL:
		printf("%s%s%s", C_RED, BALL_ICON, C_NRML);
		break;
	case LEFT_BLADE_SYMBOL:
		printf("%s%s%s", C_YLLW,LEFT_BLADE_ICON, C_NRML);
		break;
	case RIGHT_BLADE_SYMBOL:
		printf("%s%s%s", C_YLLW, RIGHT_BLADE_ICON, C_NRML);
		break;
	case TOP_WALL_SYMBOL:
		printf("%s%s%s", C_NRML, TOP_WALL_ICON, C_NRML);
		break;
	case BUTTOM_WALL_SYMBOL:
		printf("%s%s%s", C_NRML, BUTTOM_WALL_ICON, C_NRML);
		break;
	case LEFT_WALL_SYMBOL:
		printf("%s%s%s", C_NRML, BUTTOM_WALL_ICON, C_NRML);
		break;
	case RIGHT_WALL_SYMBOL:
		printf("%s%s%s", C_NRML, RIGHT_WALL_ICON, C_NRML);
		break;
	case BLANK_SYMBOL:
		//주석으로 처리한 이유는 덮어씌우기 당하기 때문이다.
		printf("%s", BLANK_ICON);
		break;
	default://다른 값이 들어오면 절대 안된다.
//		printf("%s", BLANK_ICON);
		break;
		//sthrow "Unknown SYMBOL";
	}
}
void DrawScreen::gotoxy(int x, int y){
	#ifdef USE_2BYTE_CHAR
	//ICON으로 2바이트를 사용하는 경우 2바이트씩 지우기 쓰기 위해 2칸씩 움직인다.
	COORD pos = { x << 1,y };//2*x, 각 바이트가 2바이트니깐
	#else
	COORD pos = { x,y };
	#endif
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void DrawScreen::gotoxy_for_only_2byte(int x, int y) {
	COORD pos = { x<<1 ,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void DrawScreen::set_title(string value)
{
	char temp[100];
	sprintf_s(temp, 100, "title %s", value.c_str());
	system(temp);
}
void DrawScreen::hide_scrollbar()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}
void DrawScreen::set_console_size(size_t rows, size_t cols)
{
	char temp[100];
	sprintf_s(temp, 100, "mode con cols=%d lines=%d", cols + 50, rows + 5);
	system(temp);
}
void DrawScreen::gotoxy_for_only_1byte(int x, int y) {
	COORD pos = { x ,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void DrawScreen::hide_cursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void DrawScreen::center_text(const string text[], size_t NumberOfStr, bool Line_Number, int Sort_Type) {
	
	assert(SIZE_OF_ROW_SCREEN >= NumberOfStr);
	int write_row_pos = (SIZE_OF_ROW_SCREEN - NumberOfStr) / 2;
	for (size_t i = 0; i < NumberOfStr; ++i) {
		//라인별 쓰기 시작 위치 전체 크기의 절반 - 텍스트 길이의 절반
		int each_str_size = text[i].size();
		assert(SIZE_OF_COL_SCREEN >= each_str_size);
		int write_start_col_pos = (SIZE_OF_COL_SCREEN - each_str_size) / 2; 
		gotoxy_for_only_1byte(write_start_col_pos, ++write_row_pos);
		if (Line_Number) printf("%d. %s\n", i + 1, text[i].c_str());
		else printf("%s\n", text[i].c_str());
	}
}

void DrawScreen::update_info(bool LearnMode, int GameTries, int LearnMode_AI_maxscore,
	int CompeteMode_RightAIScore, int CompeteMode_LeftPlayerScore, int Gen, bool Acceleration, bool Hide) //점수 표시하기 밑에다가
{

	hide_cursor();
	gotoxy(0, SIZE_OF_ROW_SCREEN + 1);//내려가고
	//플레이어 점수 : 10 , AI 점수 : 10 , 세대 : 1
	if (GameTries >= 0)
	{
		printf("Respawn: %d", GameTries);
	}
	if (LearnMode)
	{
		if (LearnMode_AI_maxscore >= 0) {
			printf(", AI Continous MAX Score: %d", LearnMode_AI_maxscore);
		}
		if (Gen >= 0) {
			printf(", Generation: %d", Gen);
		}
	}
	else
	{

		if (CompeteMode_LeftPlayerScore >= 0) {//점수가 0 이상인 것만 표기
			printf(", Player Score: %d", CompeteMode_LeftPlayerScore);
		}
		if (CompeteMode_RightAIScore >= 0) {
			printf(", AI Score: %d", CompeteMode_RightAIScore);
		}
	}


	if (Acceleration) printf(", Accelerating");
	if (Hide) printf(", Cleaning up blades");
	printf("                                   ");
}