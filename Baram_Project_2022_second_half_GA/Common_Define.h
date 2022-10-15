#pragma once
#include <cassert>
#include <cstdio>
#include <Windows.h>
#include <algorithm>
#include <random> //랜덤 함수
#include <Eigen/Dense>//행렬
#include <ctime> // for time(NULL) "seed for random numer genrator" 
#include <conio.h> // for _kbhit() "determining if a key is pressed"

#include <string>
#include <cstring>//strlen이나 문자열 복사등
#include <vector>
#include <utility>
#include <list>
#include <cmath>//

#include <cstdlib> //size_t
#include <random>

#define C_NRML "\033[0m"
#define C_BLCK "\033[30m"
#define C_RED  "\033[31m"
#define C_GREN "\033[32m"
#define C_YLLW "\033[33m"
#define C_BLUE "\033[34m"
#define C_PRPL "\033[35m"
#define C_AQUA "\033[36m"

#define USE_2BYTE_CHAR
#define BALL_ICON "★"
#define LEFT_BLADE_ICON "∥"
#define RIGHT_BLADE_ICON "∥"

#define TOP_WALL_ICON "▦"
#define BUTTOM_WALL_ICON "▦	"
#define LEFT_WALL_ICON "▦"
#define RIGHT_WALL_ICON "▦"
#define BLANK_ICON "　"


#define SIZE_OF_ROW_SCREEN 20
#define SIZE_OF_COL_SCREEN 40

#define MAX_Y_CAN_GO SIZE_OF_ROW_SCREEN - 2
#define MIN_Y_CAN_GO 2

#define MAX_X_CAN_GO SIZE_OF_COL_SCREEN - 2
#define MIN_X_CAN_GO 2

#define DIRECTION_COUNT 6
#define DELAY_PER_FRAME 30
//배열에 어떤값으로 저장할지 정의 하는 enum
enum ICON_NUMBER {
	BLANK_SYMBOL, BALL_SYMBOL, LEFT_BLADE_SYMBOL, RIGHT_BLADE_SYMBOL,
	TOP_WALL_SYMBOL, BUTTOM_WALL_SYMBOL, LEFT_WALL_SYMBOL, RIGHT_WALL_SYMBOL
};
enum Ball_Direction { STOP, LEFT, UPLEFT, DOWNLEFT, RIGHT, UPRIGHT, DOWNRIGHT };

struct Coor {
	int x;
	int y;
};
struct Blade_Info {
	int score;
	size_t ID_index;
};
struct Game_Info {

};

using namespace std;
using namespace Eigen;
//전역 변수로 사용하고 싶었지만 DrawScreen을 불러와야 해서 못함
//DrawScreen* dds;