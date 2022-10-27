#include "DrawScreen.h"

void DrawScreen::screen_clear()
{
	system("cls");
}

void DrawScreen::draw_layout(const ICON_NUMBER arr[][SIZE_OF_COL_SCREEN], const int row) {
	static int prv_row = 0;
	static int prv_col = 0;
	static int** prv_arr = nullptr;
	if ((prv_row != row) || (prv_col != SIZE_OF_COL_SCREEN)) {//�ƿ� ũ�� ���� �ٸ��� �ٽ� ����� �ٽ� �ؾ� �Ѵ�.
		//���� �迭�� ������ ���� �״�� ����� �� ���� �ǹ̰� �����Ƿ� �����.
		if (prv_arr != nullptr) {
			for (int k = 0; k < prv_row; k++) {
				delete[] prv_arr[k];
			}
		}

		prv_arr = new int* [row];
		for (int k = 0; k < row; k++) {
			prv_arr[k] = new int[SIZE_OF_COL_SCREEN];
		}



		//system("cls");
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < SIZE_OF_COL_SCREEN; j++) {
				gotoxy(j, i);//x, y;
				draw_convert(arr[i][j]);
			}
		}
	}
	else {//ũ�Ⱑ ������
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < SIZE_OF_COL_SCREEN; j++) {
				if (prv_arr[i][j] != arr[i][j]) {//�ٲ� �κи� �̵��ؼ� ����Ѵ�.
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
	//2k ���� �ű�� �ؾ� �Ѵ�.


	for (int i = 0; i < row; i++) {//����� �׻� ������ ������ �迭 ũ�Ⱑ ���ƾ� �Ѵ�.
		for (int j = 0; j < SIZE_OF_COL_SCREEN; j++) {
			prv_arr[i][j] = arr[i][j];
		}
	}
	prv_row = row;
	prv_col = SIZE_OF_COL_SCREEN;
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
		//�ּ����� ó���� ������ ������ ���ϱ� �����̴�.
		printf("%s", BLANK_ICON);
		break;
	default://�ٸ� ���� ������ ���� �ȵȴ�.
//		printf("%s", BLANK_ICON);
		break;
		//sthrow "Unknown SYMBOL";
	}
}
void DrawScreen::gotoxy(int x, int y){
	#ifdef USE_2BYTE_CHAR
	//ICON���� 2����Ʈ�� ����ϴ� ��� 2����Ʈ�� ����� ���� ���� 2ĭ�� �����δ�.
	COORD pos = { x << 1,y };//2*x, �� ����Ʈ�� 2����Ʈ�ϱ�
	#else
	COORD pos = { x,y };
	#endif
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void DrawScreen::gotoxy_for_only_2byte(int x, int y) {
	COORD pos = { x<<1 ,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
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
void DrawScreen::center_text(const string text[], int NumberOfStr, bool Line_Number, int Sort_Type) {
	
	assert(SIZE_OF_ROW_SCREEN >= NumberOfStr);
	int write_row_pos = (SIZE_OF_ROW_SCREEN - NumberOfStr) / 2;
	for (int i = 0; i < NumberOfStr; i++) {
		//���κ� ���� ���� ��ġ ��ü ũ���� ���� - �ؽ�Ʈ ������ ����
		int each_str_size = text[i].size();
		assert(SIZE_OF_COL_SCREEN >= each_str_size);
		int write_start_col_pos = (SIZE_OF_COL_SCREEN - each_str_size) / 2; 
		gotoxy_for_only_1byte(write_start_col_pos, ++write_row_pos);
		if (Line_Number) printf("%d. %s\n", i + 1, text[i].c_str());
		else printf("%s\n", text[i].c_str());
	}
}

void DrawScreen::update_info(bool LearnMode, int GameTries, int LearnMode_AI_maxscore,
	int CompeteMode_RightAIScore, int CompeteMode_LeftPlayerScore, int Gen, bool Acceleration, bool Hide) //���� ǥ���ϱ� �ؿ��ٰ�
{

	hide_cursor();
	gotoxy(0, SIZE_OF_ROW_SCREEN + 1);//��������
	//�÷��̾� ���� : 10 , AI ���� : 10 , ���� : 1
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

		if (CompeteMode_LeftPlayerScore >= 0) {//������ 0 �̻��� �͸� ǥ��
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