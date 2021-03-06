#include "ghosts.h"
#include "pac_man.h"
#include "pellets.h"
#include "matrix.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;


const int r = 23, c = 29;

int main()
{


	RenderWindow window; //creation of window
	window.create(VideoMode(1160, 1020), "Pacman");
	window.setFramerateLimit(5); //setting speed of window

	//music and sound effects
	Music intro;
	intro.openFromFile("pacman_beginning.wav");
	SoundBuffer chomp_buffer;
	chomp_buffer.loadFromFile("pacman_chomp.wav");
	Sound chomp;
	chomp.setBuffer(chomp_buffer);
	SoundBuffer ghost_eaten_buffer;
	ghost_eaten_buffer.loadFromFile("pacman_eatghost.wav");
	Sound eat_ghost;
	eat_ghost.setBuffer(ghost_eaten_buffer);
	SoundBuffer intermission_buffer;
	intermission_buffer.loadFromFile("pacman_intermission.wav");
	Sound intermission;
	intermission.setBuffer(intermission_buffer);
	SoundBuffer death_buffer;
	death_buffer.loadFromFile("pacman_death.wav");
	Sound death;
	death.setBuffer(death_buffer);

	Clock clock;
	Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		cout << "Error in font file";
	};

	//drawing ready text on sfml
	Text text;
	text.setFont(font);
	text.setString("READY!");
	text.setFillColor(Color::Yellow);
	text.setPosition(Vector2f(500, 525));

	// drawing lives text
	Text text_lives;
	text_lives.setFont(font);
	text_lives.setString("Lives:");
	text_lives.setFillColor(Color::White);
	text_lives.setPosition(Vector2f(800, 920));
	Text number;
	number.setFont(font);
	number.setFillColor(Color::White);
	number.setPosition(Vector2f(1000, 920));

	//drawing score text
	Text score_text;
	Text score_number;
	score_text.setFont(font);
	score_number.setFont(font);
	score_text.setString("Score:");
	score_text.setFillColor(Color::White);
	score_number.setFillColor(Color::White);
	score_text.setPosition(Vector2f(100, 920));
	score_number.setPosition(Vector2f(300, 920));

	int lives = 3; //max lives of pacman
	int eaten = 0; //to count eaten pellets
	int score = -10; //to get score (starting with -10 since there is a pellet drawn at the initial position of pacman)
	//small pellets 10 points, large pellets 50 points
	Clock fright;
	int arr[r][c]; //array of board
	int pell_arr[r][c]; //array of pellets
	int pellet_count = 0;
	int count; //used in drawing pellet
	bool is_fright_on = false;
	char dir = '/'; //direction of pacman

	ifstream inputFile;
	inputFile.open("Board.txt");
	if (inputFile.fail()) // if file fails
	{
		cout << "Error with board file" << endl;
		exit(0);
	}

	if (inputFile.is_open()) //input of file
	{
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
			{
				inputFile >> arr[i][j];
				if (arr[i][j] != -11 && arr[i][j] != -22 &&
					arr[i][j] != 032 && arr[i][j] != 037 &&
					arr[i][j] != 238 && arr[i][j] != 243
					&& arr[i][j] != 122 && arr[i][j] != 135
					&& arr[i][j] != 136 && arr[i][j] != 137 && arr[i][j] != 138 && arr[i][j] != 139
					&& arr[i][j] != 140 && arr[i][j] != 141)
				{
					pell_arr[i][j] = 1; //setting array of pellets
					pellet_count++;

				}
				else if (arr[i][j] == 032 || arr[i][j] == 037 || arr[i][j] == 238 || arr[i][j] == 243)
				{
					pell_arr[i][j] = 2;
					pellet_count++;
				}
				else
					pell_arr[i][j] = 0;
			};
	};
	

	Matrix mat(arr);
	vector<vector<int>> paths =mat.dijkstra(arr,13,11,14,17);
	int i = 218;
	
		
	

	inputFile.close();

	//intitializations
	pac_man pac(17, 14, "pacman.png", "pacman2.png");

	ghosts blinky(9, 14, "blinky.png");
	ghosts inky(11, 13, "inky.png");
	ghosts pinky(11, 14, "pinky.png");
	ghosts clyde(11, 15, "clyde.png");

	pellets* pelletsptr = new pellets[pellet_count];
	pellets power;

	//constructing board SFML
	RectangleShape board[r][c];
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
		{
			board[i][j].setPosition(40 * j, 40 * i);
			board[i][j].setSize(Vector2f(40, 40));
			if (arr[i][j] == -11)
			{
				board[i][j].setFillColor(Color::Blue);
			}
			else
			{
				board[i][j].setFillColor(Color::Black);
			}
		}

	Event e;
	intro.play();
	
	while (window.isOpen())
	{
		
			vector<vector<int>> path_inky = mat.dijkstra(arr, inky.getColumn(), inky.getRow(), 14, 17);
			
				int i = arr[pac.getRow()][pac.getColumn()];

		
			int j_inky = path_inky[i].size() - 2;

			vector<vector<int>> path_blinky = mat.dijkstra(arr, blinky.getColumn(), blinky.getRow(), 14, 17);
			int j_blinky = path_blinky[i].size() - 2;

			vector<vector<int>> path_clyde = mat.dijkstra(arr, clyde.getColumn(), clyde.getRow(), 14, 17);
			int j_clyde = path_clyde[i].size() - 2;

			vector<vector<int>> path_pinky = mat.dijkstra(arr, pinky.getColumn(), pinky.getRow(), 14, 17);
			int j_pinky = path_pinky[i].size() - 2;
		
			
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();


			else if (e.type == Event::KeyPressed)

			{
				switch (e.key.code)  //input from user to change direction of pacman
				{
				case Keyboard::Up:
					pac.rotate('U');
					dir = 'U';
					break;

				case Keyboard::Down:
					pac.rotate('D');
					dir = 'D';
					break;

				case Keyboard::Right:
					pac.rotate('R');
					dir = 'R';
					break;

				case Keyboard::Left:
					pac.rotate('L');
					dir = 'L';
					break;
				};

			};

		};

		//incase of collision with ghosts
		if (!is_fright_on)
			if ((pac.getColumn() == pinky.getColumn() && pac.getRow() == pinky.getRow())
				|| (pac.getColumn() == blinky.getColumn() && pac.getRow() == blinky.getRow())
				|| (pac.getColumn() == inky.getColumn() && pac.getRow() == inky.getRow()) ||
				(pac.getColumn() == clyde.getColumn() && pac.getRow() == clyde.getRow()))
			{
				death.play();
				lives--;
				dir = '/';
				pac.restart();
				blinky.home(9, 14);
				inky.home(11, 13);
				pinky.home(11, 14);
				clyde.home(11, 15);

			}

		//incase fright mode is on
		if (is_fright_on)
		{
			if ((pac.getColumn() == pinky.getColumn() && pac.getRow() == pinky.getRow()))
			{
				score = score + 200;
				pinky.home(11, 14);
				eat_ghost.play();

			}
			if ((pac.getColumn() == blinky.getColumn() && pac.getRow() == blinky.getRow()))
			{
				blinky.home(9, 14);
				score = score + 200;
				eat_ghost.play();
			}
			if ((pac.getColumn() == inky.getColumn() && pac.getRow() == inky.getRow()))
			{
				inky.home(11, 13);
				score = score + 200;
				eat_ghost.play();
			}
			if ((pac.getColumn() == clyde.getColumn() && pac.getRow() == clyde.getRow()))
			{
				clyde.home(11, 15);
				score = score + 200;
				eat_ghost.play();
			}
		}


		//case of all lives are lost
		if (lives == 0)
		{
			cout << "You lost!";
			cout << endl;
			cout << "Duration of game " << clock.getElapsedTime().asSeconds() << "s";
			cout << endl;
			cout << "Your score is " << score;
			window.close();
		};

		//rotates pacman based on direction
		if ((pac.getRow() == 11) && (pac.getColumn() == 28) && (dir == 'R'))
			pac.teleport_left(); //handles if pacman is on the far left and teleports on the other side of board
		else
			if ((pac.getRow() == 11) && (pac.getColumn() == 0) && (dir == 'L'))
				pac.teleport_right(); //handles if pacman is on the far right and teleports on the other side of board
			else
				pac.move(dir, arr);

		if (dir != '/')
		{
			pac.alternate();

			if (is_fright_on)
				pinky.moveOnWindow(arr);
			else
			pinky.move_gedeed(arr, path_pinky[i][j_pinky]);
			if (clock.getElapsedTime().asSeconds() > 5)
			{
				if (is_fright_on)
					blinky.moveOnWindow(arr);
				else
				blinky.move_gedeed(arr, path_blinky[i][j_blinky]);
			}

			if (clock.getElapsedTime().asSeconds() > 10) //moves after 5 seconds
			{
				if (is_fright_on)
					inky.moveOnWindow(arr);
				else
				inky.move_gedeed(arr, path_inky[i][j_inky]);
			}

			if (clock.getElapsedTime().asSeconds() > 20) //moves after 8 seconds
			{
				if (is_fright_on)
					clyde.moveOnWindow(arr);
				else
				clyde.move_gedeed(arr, path_clyde[i][j_clyde]);
			}
		}
		window.clear();



		if (pell_arr[pac.getRow()][pac.getColumn()] == 1) //handles if pacman eats a normal pellet
		{
			pell_arr[pac.getRow()][pac.getColumn()] = 0;
			eaten++;
			score = score + 10;
			chomp.play();
		}

		if (pell_arr[pac.getRow()][pac.getColumn()] == 2) //handle if pacman eats a power pellet
		{
			score = score + 50;
			eaten++;
			is_fright_on = true;
			fright.restart();
			chomp.play();
			intermission.play();
		}

		if (fright.getElapsedTime().asSeconds() < 9 && is_fright_on) //it takes 9 seconds for the ghosts to get back to their normal routine
		{
			pell_arr[pac.getRow()][pac.getColumn()] = 0;
			inky.Fright_Mode("fright.png");
			pinky.Fright_Mode("fright.png");
			blinky.Fright_Mode("fright.png");
			clyde.Fright_Mode("fright.png");

			//Reversing the dijkstra


			if (pac.getColumn() == inky.getColumn() && pac.getRow() == inky.getRow())
			{
				score = score + 200;
				inky.Fright_Mode("eyes.png");
			}
			else if (pac.getColumn() == pinky.getColumn() && pac.getRow() == pinky.getRow())
			{
				score = score + 200;
				pinky.Fright_Mode("eyes.png");
			}
			else if (pac.getColumn() == clyde.getColumn() && pac.getRow() == clyde.getRow())
			{
				score = score + 200;
				clyde.Fright_Mode("eyes.png");
			}
			else if (pac.getColumn() == blinky.getColumn() && pac.getRow() == blinky.getRow())
			{
				score = score + 200;
				blinky.Fright_Mode("eyes.png");
			}
		}
		else
		{
			is_fright_on = false;
			blinky.Fright_Mode("blinky.png");
			pinky.Fright_Mode("pinky.png");
			inky.Fright_Mode("inky.png");
			clyde.Fright_Mode("clyde.png");

		}
		// The Dijkstra's works normally


		if (eaten == pellet_count) //winning condition
		{
			cout << "You win!";
			cout << endl;
			cout << "Duration of game " << clock.getElapsedTime().asSeconds() << "s";
			cout << endl;
			cout << "Your score is " << score;
			window.close();
		}

		// Drawing:
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
				window.draw(board[i][j]); //draws board

		count = 0; //drawing pellets
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
			{
				if (pell_arr[i][j] == 1)
				{
					pelletsptr[count].setposition(40 * j + 10, 40 * i + 10);
					pelletsptr[count].drawOnWindow(window);
					count++;
				};
				if (pell_arr[i][j] == 2)
				{
					power.setposition(40 * j + 10, 40 * i + 10);
					power.load("power.png");
					power.drawOnWindow(window);

				};
			};

		if (dir == '/')
			window.draw(text);
		window.draw(text_lives);
		number.setString(to_string(lives));
		window.draw(number);
		window.draw(score_text);
		score_number.setString(to_string(score));
		window.draw(score_number);
		pac.drawonWindow(window);
		pinky.drawOnWindow(window);
		inky.drawOnWindow(window);
		blinky.drawOnWindow(window);
		clyde.drawOnWindow(window);

		window.display();
	};

	return 0;

};


void create_matrix(int board[r][c],int m[276][276])
{
	for (int matrix_row = 0; matrix_row < 276; matrix_row++) //matrix row
	{
		for (int board_row = 0; board_row < r; board_row++) // board row
		{
			for (int board_column = 0; board_column < c; board_column++) //board column
			{
				if (board[board_row][board_column] == matrix_row) //finds matrix row and array of board
				{
					for (int matrix_column = 0; matrix_column < 276; matrix_column++) //matrix column
					{
						if (board[board_row + 1][board_column] == matrix_column)
							m[matrix_row][matrix_column] = 1;
						else if (board[board_row - 1][board_column] == matrix_column)
							m[matrix_row][matrix_column] = 1;
						else if (board[board_row][board_column + 1] == matrix_column)
							m[matrix_row][matrix_column] = 1;
						else if (board[board_row][board_column + 1] == matrix_column)
							m[matrix_row][matrix_column] = 1;
						else if (board[board_row][board_column - 1] == matrix_column)
							m[matrix_row][matrix_column] = 1;
						else
							m[matrix_row][matrix_column] = 0;


					}


				}
			}
		}

	}
}
