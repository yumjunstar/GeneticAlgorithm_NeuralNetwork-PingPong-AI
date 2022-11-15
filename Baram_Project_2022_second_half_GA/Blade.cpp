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

	// 처음에 거리를 최대 값으로 설정해야 한다.
	// 닿을때마다 업데이트 한다.
	// 한번도 안 부딫힌 것은 -1로 초기화 한다.
	// 부딫치면 0으로 초기화 한뒤 값을 더한다.


	// 이렇게 하면 안됨 그냥 오른쪽 벽에 닿으면 그 시점에서 모두 죽으니깐 죽은 시점에서 마지막 위치를 더할 것임
	this->last_distance_from_ball = 0;
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
	else y = MoveRangeStart_y;
	//회전해봤지만 탁구의 취지에 안맞는 것 같다 조금만 움직이면 되니깐 금방 학습이 끝난다. 한방향으로만 움직임
}
void Blade::blade_move_down() {
	int sum = y + this->speed;
	if (sum <= MoveRangeEnd_y) y = sum;
	else y = MoveRangeEnd_y;
}
pair<int, int> Blade::GetBladeRange()
{
	int blade_y_range_start = this->y - (this->blade_size >> 1);
	int blade_y_range_end = this->y + (this->blade_size >> 1);
	return make_pair(blade_y_range_start, blade_y_range_end);
}
void Blade::UpdateBetweenBallAndMeDistance(Coor ball)
{
	// 일반화 하기 위해 2차원 거리를 구하기
	last_distance_from_ball = sqrt(pow(ball.x - this->x, 2) + pow(ball.y - this->y, 2));
}
void Blade::UpdateBetweenBallAndMeDistance(double distance)
{
	last_distance_from_ball = distance;
}

void Blade::AddBetweenBallAndMeDistance(double distance)
{
	last_distance_from_ball += distance;
}

double Blade::GetBetweenBallAndMeDistance()
{
	return last_distance_from_ball;
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
