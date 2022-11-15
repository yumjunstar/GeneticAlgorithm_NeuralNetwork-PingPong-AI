#include "DrawScreen.h"
#include "Ball.h"
#include "Blade.h"
#include "PingPong.h"
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "Common_Define.h"
#include "Menu.h"
#include "FileManage.h"
void init_setting();

int main(void) {
	init_setting();
	DrawScreen *dds = new DrawScreen;
	FileManage* fm = new FileManage;
	Menu m(dds, fm);
	m.StartMenu(100);
	delete dds;
	delete fm;
}
void init_setting()
{

	//10%정도는 그대로 남기기 그렇지 않으면 자꾸 새로운 돌연변이가 생겨서
	//결과를 망치는 것 같다.
	//그냥 끝에 가만히 있는 것들도 들어온다.
	//멀티 스레딩으로 화면 출력하기
	srand(time(NULL));
	char temp[100];
	sprintf_s(temp, 100, "mode con cols=%d lines=%d | title %s", SIZE_OF_COL_SCREEN + 50, SIZE_OF_ROW_SCREEN + 10, "Ping Pong AI");
	system(temp);
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	//system("color 42");
}