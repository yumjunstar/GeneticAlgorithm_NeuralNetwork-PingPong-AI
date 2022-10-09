#include "Blade.h"

// constructor


Blade::Blade(int x, int y, int ID, int BladeMoveRangeStart_y, int BladeMoveRangeEnd_y, 
	int BladeSize, int BladeSpeed) {
	this->initial_x = x;
	this->initial_y = y;
	this->x = x;
	this->y = y;

	this->blade_size = BladeSize;
	this->id = ID;
	this->speed = BladeSpeed;
	this->score = 0;
	this->CountinusMAXScoreBeforeDeath = 0;
	this->MoveRangeStart_y = BladeMoveRangeStart_y;
	this->MoveRangeEnd_y = BladeMoveRangeEnd_y;

}

// to reset blade positions while Ping_Pong restarting

void Blade::blade_reset() {
	x = initial_x;
	y = initial_y;
}

// to move balde up

void Blade::blade_move_up() {//일정 한도부터는 못가게 또는 뱅글 뱅글 돌게 해도 괜찮을 것 같은데
	int sum = y - this->speed;
	if (sum >= MoveRangeStart_y) y = sum;
	//else y = MoveRangeEnd_y;
	//회전해봤지만 탁구의 취지에 안맞는 것 같다 조금만 움직이면 되니깐 금방 학습이 끝난다. 한방향으로만 움직임
}
void Blade::blade_move_down() {
	int sum = y + this->speed;
	if (sum <= MoveRangeEnd_y) y = sum;
	//else y = MoveRangeStart_y;
}
pair<int, int> Blade::GetBladeRange()
{
	int blade_y_range_start = this->y - (this->blade_size >> 1);
	int blade_y_range_end = this->y + (this->blade_size >> 1);
	return make_pair(blade_y_range_start, blade_y_range_end);
}
bool Blade::is_hit_blade(int check_x, int check_y)
{
	//y - blade_size / 2
	int blade_y_range_start = this->y - (blade_size >> 1);
	int blade_y_range_end = this->y + (blade_size >> 1);
	assert(blade_y_range_start >= 0);
	assert(blade_y_range_end < SIZE_OF_ROW_SCREEN);
	if (check_x == this->x)
	{
		if ((check_y <= blade_y_range_end) && (check_y >= blade_y_range_start))
		{
			return true;
		}
		else return false;
	}
	else return false;
}

int Blade::GetCountinusMAXScore()
{
	return this->CountinusMAXScoreBeforeDeath;
}

void Blade::CountinusMAXScore_SetScore(int score)
{
	this->CountinusMAXScoreBeforeDeath = score;
}

void Blade::SetRandomizeBlade_InitialYPos()
{
	initial_y = (rand() % (this->MoveRangeEnd_y - this->MoveRangeStart_y)) + this->MoveRangeStart_y;
}

// to move balde down



void Blade::add_score(int score) {
	this->score += score;
	if (this->score > this->CountinusMAXScoreBeforeDeath)
	{
		this->CountinusMAXScoreBeforeDeath = this->score;
	}

}

void Blade::set_score(int score) {
	this->score = score;
	if (this->score > this->CountinusMAXScoreBeforeDeath)
	{
		this->CountinusMAXScoreBeforeDeath = this->score;
	}
}

int Blade::get_score() {
	return this->score;
}

int Blade::GetID()
{
	return this->id;
}

size_t Blade::get_blade_size()
{
	return this->blade_size;
}

Coor Blade::GetBladeCoordinate(){
	return Coor({x, y});
}
