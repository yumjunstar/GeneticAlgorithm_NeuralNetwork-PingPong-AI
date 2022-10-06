#include "DrawScreen.h"
#include "Ball.h"
#include "Blade.h"
#include "PingPong.h"
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "Common_Define.h"
#include "Menu.h"
int main(void) {
	char temp[100];
	sprintf_s(temp, 100, "mode con cols=%d lines=%d | title %s", SIZE_OF_COLUMN_SCREEN + 50, SIZE_OF_ROW_SCREEN + 10, "Ping Pong AI");
	system(temp);
	//DrawScreen* ds = new DrawScreen();
	Menu m;
	m.StartMenu();
	//delete ds;
}