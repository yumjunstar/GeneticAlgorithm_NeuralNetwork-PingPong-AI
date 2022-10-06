//// required imports
//#define C_NRML "\033[0m"
//#define C_BLCK "\033[30m"
//#define C_RED  "\033[31m"
//#define C_GREN "\033[32m"
//#define C_YLLW "\033[33m"
//#define C_BLUE "\033[34m"
//#define C_PRPL "\033[35m"
//#define C_AQUA "\033[36m"
//
//
//using namespace std; // for cout
//
//// global variable for specifying direction of the ball
//
//typedef vector< vector<int>> matrix;
//struct Blade_Info {
//	int own_number;
//	vector<matrix> weight;
//	Blade_Info operator +(const Blade_Info &a) {
//		int size_a = a.weight.size();
//		int size_this = this->weight.size();
//		assert(size_a == size_this);
//		Blade_Info temp_Blade_Information;
//		vector <matrix> new_weight_matrix_vector;
//
//		for (int i = 0; i < size_a; i++) {//가중치 묶음의 개수가 같다.
//			int row_a = a.weight[i].size();//각 matrix의 행의 크기
//			int col_a = a.weight[i][0].size();//각 matrix의 열의 크기
//
//			int row_this = this->weight[i].size();//각 matrix의 행의 크기
//			int col_this = this->weight[i][0].size();//각 matrix의 열의 크기
//
//			assert(row_a == row_this);//같아야 한다.
//			assert(col_a == col_this);
//
//			matrix temp_weight_matrix(row_a, vector<int>(col_a, 0));
//			for (int row = 0; row < row_a; row++) {
//				for (int col = 0; col < col_a; col++) {
//					temp_weight_matrix[row][col] = a.weight[i][row][col] + this->weight[i][row][col];
//				}
//			}
//			new_weight_matrix_vector.push_back(temp_weight_matrix);
//		}
//
//		assert(a.weight.size() == this->weight.size());//일단 행이 같은지 확인
//		//직사각형 행렬이라 가정 하고
//
//	}
//};
//
//
//
//// Ball Class
//class Ball {
//public:
//
//	int x, y, center_x, center_y; // to hold postions (current and center)
//	Ball_Direction direction; // to hold direction of motion of ball
//
//
//	// constructor
//	Ball(int x, int y) { // initial position of the ball
//		this->center_x = x; // center_X and center_y store the initial value
//		this->center_y = y;
//		this->x = x; // x and y store current value of the ball position
//		this->y = y;
//		this->direction = STOP; // ball is initialized static
//	}
//
//	// function to bring the ball back to its starting position (center)
//	void reset_ball() {
//		x = center_x;
//		y = center_y;
//		direction = STOP; // to stop the motion of ball
//	}
//
//	// function to change direction of ball when it hits either a wall or a blade
//	void change_ball_direction(Ball_Direction d) {
//		direction = d;
//	}
//
//	// function to initialize the direction of ball randomely
//	void randomize_ball_direction() {
//		direction = (Ball_Direction)((rand() % 6) + 1);
//	}
//
//	// function to move ball around the screen
//	void move_ball() { // x increases along right
//		switch (direction) { // y increases along bottom
//		case STOP:
//			break;
//		case LEFT:
//			x--;
//			break;
//		case RIGHT:
//			x++;
//			break;
//		case UPLEFT:
//			x--;
//			y--;
//			break;
//		case DOWNLEFT:
//			x--;
//			y++;
//			break;
//		case UPRIGHT:
//			x++;
//			y--;
//			break;
//		case DOWNRIGHT:
//			x++;
//			y++;
//			break;
//		default: // in case direction take execptional value, ignore it.
//			break;
//		}
//	}
//}; // end of Ball class
//
//
//// Blade Class
//class Blade {
//public:
//	int x, y, initial_x, initial_y; // to hold position of blades (current and default)
//
//	// constructor
//	Blade(int x, int y) {
//		this->initial_x = x;
//		this->initial_y = y;
//		this->x = x;
//		this->y = y;
//	}
//
//	// to reset blade positions while Ping_Pong restarting
//	void blade_reset() {
//		x = initial_x;
//		y = initial_y;
//	}
//
//	// to move balde up
//	void blade_move_up() {
//		y--;
//	}
//
//	// to move balde down
//	void blade_move_down() {
//		y++;
//	}
//}; // end of Blade class
//
//
//
//// Ping_Pong Class
//class Ping_Pong {
//public:
//	int width, height, score_1, score_2;// height and width of the Ping_Pong area
//										// scores  of player 1 and player 2
//	char up_1, down_1, up_2, down_2; // keys used by players to move up or down
//	bool terminate; // to terminate the Ping_Pong
//
//	Ball* ball; // 1 ball
//	Blade* player_1, * player_2; // 2 players
//	//const static int blades_count = 100;
//	//Blade* blades[blades_count];
//
//	// constructor 	
//	Ping_Pong(int width, int height) {
//		srand(time(NULL)); // seeding random number generator to generate random directions
//
//		// initializing variables
//		this->terminate = false;
//		this->up_1 = 'w'; // player_1 move up using 'w'
//		this->up_2 = 'i'; // player_2 move up using 'i'
//		this->down_1 = 's'; // player_1 move down using 's'
//		this->down_2 = 'k'; // player_2 move down using 'k'
//		this->score_1 = 0; // both players get initial score = 0
//		this->score_2 = 0;
//
//		this->width = width; // user can specify the dimesions of the grid
//		this->height = height;
//
//		this->ball = new Ball(width / 2, height / 2);
//		this->player_1 = new Blade(1, height / 2 - 3);
//		this->player_2 = new Blade(width - 2, height / 2 - 3);
//		//for (int i = 0; i < blades_count; i++) {
//		//	this->blades[i] = new Blade(width - 2, i % height);
//		//}
//	}
//
//	// Incrementing score
//	void increment_score(Blade* player) {
//		if (player == player_1) score_1 += 5; // increment player 1 score
//		else score_2 += 5;// increment player 2 score
//
//		ball->reset_ball(); // put ball back at center
//		player_1->blade_reset(); // put both blades at starting position
//		player_2->blade_reset();
//	}
//
//	// Drawing the board (at each moment -- this will explain the blips)
//	void draw_layout() {
//		system("cls"); // first clearing the screen to remove prev timestep
//
//		// printing above wall
//		for (int i = 0; i < width + 2; i++) printf("-");
//		printf("\n");
//
//		// printing side walls, ball and blades
//		for (int i = 0; i < height; i++) { // to traverse along height
//			for (int j = 0; j < width; j++) { // to traverse along width
//
//				if (j == 0) printf("|"); // left wall element
//
//				if (ball->x == j && ball->y == i) printf("%sO%s",C_RED, C_NRML); // printing ball at its
//															// position
//				// printing player_1 blade (length=4)
//				else if (player_1->x == j && player_1->y == i) printf("%s]%s",C_YLLW, C_NRML);
//				else if (player_1->x == j && player_1->y + 1 == i) printf("%s]%s", C_YLLW, C_NRML);
//				else if (player_1->x == j && player_1->y + 2 == i) printf("%s]%s", C_YLLW, C_NRML);
//				else if (player_1->x == j && player_1->y + 3 == i) printf("%s]%s", C_YLLW, C_NRML);
//
//				// printing player_2 blade (length=4)
//				else if (player_2->x == j && player_2->y == i) printf("%s[%s", C_YLLW, C_NRML);
//				else if (player_2->x == j && player_2->y + 1 == i) printf("%s[%s", C_YLLW, C_NRML);
//				else if (player_2->x == j && player_2->y + 2 == i) printf("%s[%s", C_YLLW, C_NRML);
//				else if (player_2->x == j && player_2->y + 3 == i) printf("%s[%s", C_YLLW, C_NRML);
//
//
//				else printf(" "); // rest of the area is blank
//
//				if (j == width - 1) printf("|"); // right wall element
//			} // end of inner for loop
//
//			printf("\n");
//		} // end of outer for loop
//
//		// printing bottom wall
//		for (int i = 0; i < width + 2; i++) printf("_");
//		printf("\n");
//
//		// printing scores
//		//printf("Player Score: %d \t     AI Score : %d\n", score_1, score_2);
//		printf("Generation: %d \t Score: %d", 1, 0);
//
//	} // end of Draw function
//
//	// function to respond to player inputs
//	void play() {
//		ball->move_ball(); // to move the ball in direction specified by 'direction'
//
//		if (_kbhit()) { // if any key is pressed : take action
//			char key = _getch(); // get the pressed key character
//
//			// player_1 move up
//			if (key == up_1 && player_1->y > 0) player_1->blade_move_up();
//			// player_2 move up	
//			else if (key == up_2 && player_2->y > 0) player_2->blade_move_up();
//			// player_1 move down
//			else if (key == down_1 && player_1->y + 4 < height) player_1->blade_move_down();
//			// player_2 move down
//			else if (key == down_2 && player_2->y + 4 < height) player_2->blade_move_down();
//			// terminate if 't' is pressed
//			else if (key == 't') terminate = true;
//
//			// if it's new game move the ball in random directions				
//			if (ball->direction == STOP) ball->randomize_ball_direction();
//		}
//	} // end of input
//
//	// function to moniter ball position
//	void monitor_ball() {
//
//		// if ball hits player_1 blade
//		for (int i = 0; i < 4; i++)
//			if (ball->x == player_1->x + 1)
//				if (ball->y == player_1->y + i)
//					ball->change_ball_direction((Ball_Direction)(rand() % 3 + 4));
//
//		// if ball hits player_2 blade
//		for (int i = 0; i < 4; i++)
//			if (ball->x == player_2->x - 1)
//				if (ball->y == player_2->y + i)
//					ball->change_ball_direction((Ball_Direction)(rand() % 3 + 1));
//
//		// if ball hits bottom wall
//		if (ball->y == height - 1)
//			ball->change_ball_direction(ball->direction == DOWNRIGHT ? UPRIGHT : UPLEFT);
//
//		// if ball hits top wall
//		if (ball->y == 0)
//			ball->change_ball_direction(ball->direction == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
//
//		// if ball hits right wall --> player_1 wins
//		if (ball->x == width - 1)
//			increment_score(player_1);
//
//		// if ball hits left wall --> player_2 wins
//		if (ball->x == 0)
//			increment_score(player_2);
//
//	}
//
//	// function to call the functions --> yeah that's pretty much it
//	void lets_ping_pong() {
//		while (!terminate) {
//			draw_layout();
//			play();
//			monitor_ball();
//		}
//	}
//};
//

//
//// driver function
//int main() {
//	hidecursor();
//	Ping_Pong game(40, 20); // instanciating game
//	game.lets_ping_pong(); // playing
//	return 0; // leaving
//}
