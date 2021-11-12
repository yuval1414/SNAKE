//Yuval Hochman 318430717
//The program is the game of snake, shows a menu, the player choose a level and the game begen when the player press a direction key.
//to win the game the player need to eat 10 $ (food) , if the snake hits a wall or itself you lose.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key

typedef struct
{//struct of each vertebra of the snake, the coordinate on the board and its direction
	int row;
	int col;
	char newDir;
	char oldDir;
}coordinate;

void gotoxy(int y, int x);
int getKey();
//srand(int time);
coordinate* creatSnake(int snakeSize);
void inItSnake(coordinate* snake, int snakeSize, int direction);
void moveSnake(char newDirection, coordinate* snake, int arrCell);
void goRight(coordinate* snake, int arrCell);
void goLeft(coordinate* snake, int arrCell);
void goDown(coordinate* snake, int arrCell);
void goUp(coordinate* snake, int arrCell);
char move(coordinate* snake, int snakeSize);
void food(coordinate* snake, int snakeSize, int* foodRow, int* foodCol);
int snakeEats(coordinate* snake, int snakeSize, int* foodRow, int* foodCol);
coordinate* widenedSnake(coordinate* snake, int snakeSize);
void newCellInIt(coordinate* snake, int snakeSize);
void printMenu();
void winGame();
void loseGame();
void printFrame();
bool hitBorder(coordinate* snake);
bool goToParallelDir(coordinate* snake);
bool hitItSelf(coordinate* snake, int snakeSize);
int levelOfGame();
void mainBoard(int startx, int starty, char direction, int* foodRow, int* foodCol, int snakeSize, bool lose, bool win);
int play(char direction, coordinate* snake, int snakeSize, int* foodRow, int* foodCol, bool lose, bool win);



int main()
{
	bool lose = false, win = false;
	int snakeSize = 1;
	char direction = RIGHT;
	int startx = 40, starty = 12;  // x for coulmn and y for line 
	int foodrow = 0, foodcol = 0;
	int* foodRow, * foodCol;
	foodRow = &foodrow;
	foodCol = &foodcol;
	mainBoard(startx, starty, direction, foodRow, foodCol, snakeSize, lose, win);
}
// The menu and if win/lose
void mainBoard(int startx, int starty, char direction, int* foodRow, int* foodCol, int snakeSize, bool lose, bool win)
{//the main board is the loop that every time a player wins or loses shows the menu and the player can play again
	int statusGame = 3, gameLevel = 9;
	while (true)
	{
		coordinate* snake = creatSnake(snakeSize);
		while (gameLevel != 1) {
			gameLevel = levelOfGame();
			system("cls");
		}//system("cls");
		printFrame();
		food(snake, snakeSize, foodRow, foodCol);
		gotoxy(startx, starty);
		_kbhit();// if any key was hit
		direction = getKey();  // change direction
		inItSnake(snake, snakeSize, direction);
		direction = move(snake, snakeSize);
		statusGame = play(direction, snake, snakeSize, foodRow, foodCol, lose, win);
		if (statusGame == 0) {
			loseGame();
			gotoxy(0, 3);
		}
		else if (statusGame == 1) {
			winGame();
			gotoxy(0, 3);
		}
		else
			system("cls");

		gameLevel = 9;
	}
}
//The actual game run
int play(char direction, coordinate* snake, int snakeSize, int* foodRow, int* foodCol, bool lose, bool win)
{//the duration of a game runs in this loop
	bool hitWall = false, hitSelf = false, wrongDir = false;
	int countTurns = 1, count = 0;
	while (direction != ESC && lose == false && win == false)
	{
		// sleeps for half a second(500) before continuing to the next command
		Sleep(500);
		if (count < 10)
		{
			snakeSize++;
			snake = widenedSnake(snake, snakeSize);
			snake[snakeSize - 1].newDir = snake[snakeSize - 2].oldDir;
			snake[snakeSize - 1].oldDir = snake[snakeSize - 2].oldDir;
			newCellInIt(snake, snakeSize);
			count++;
		}
		if (*foodRow == snake[0].row && *foodCol == snake[0].col) {// foodrow and foodcol opposite
			snakeSize = snakeEats(snake, snakeSize, foodRow, foodCol);
			countTurns++;
		}
		if (countTurns == 10)
			win = true;
		//move snake and return the new direction the snake is going in 
		direction = move(snake, snakeSize);
		hitWall = hitBorder(snake);// if the snake stuck in a border or itself
		wrongDir = goToParallelDir(snake);// if the snake go to the paralle direction we lose
		hitSelf = hitItSelf(snake, snakeSize);
		if (hitWall == true || wrongDir == true || hitSelf == true)
			lose = true;


		gotoxy(26, 81);
	}
	free(snake);
	if (lose == true)
		return 0;
	else if (win == true)
		return 1;
	else
		return 2;
}
// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

void inItSnake(coordinate* snake, int snakeSize, int direction)
{//enter values to the cells in the array 
	//int i, cell;
	snake[0].row = 12;
	snake[0].col = 35;
	snake[0].oldDir = direction;
	snake[0].newDir = direction;
}
coordinate* creatSnake(int snakeSize)
{//creat a dynamic array for the snake
	coordinate* snake = (coordinate*)malloc(snakeSize * sizeof(coordinate));

	if (snake == NULL) {
		printf("error in array");
		exit(0);
	}
	return snake;
}
void moveSnake(char newDirection, coordinate* snake, int arrCell)
{//by the direction the snake need to go to we go to the needed func that changes the value of the cells 
	switch (newDirection)
	{
	case RIGHT:
		goRight(snake, arrCell);
		break;
	case DOWN:
		goDown(snake, arrCell);
		break;
	case LEFT:
		goLeft(snake, arrCell);
		break;
	case UP:
		goUp(snake, arrCell);
		break;
	}
}
char move(coordinate* snake, int snakeSize)
{//the func update the value of the direction each cell goes to by depending on the previous cell, and return the direction of the snake
	int cell;
	int key;
	for (cell = 0; cell < snakeSize; cell++)
	{
		if (cell == 0) {
			if (_kbhit())// if any key was hit
			{
				key = getKey();
				if (key == ESC) {
					snake[cell].newDir = key;
					return snake[cell].newDir;
				}
				if (key == RIGHT || key == LEFT || key == DOWN || key == UP) {
					snake[cell].oldDir = snake[cell].newDir;
					snake[cell].newDir = key;  // change direction
				}
				moveSnake(snake[cell].newDir, snake, cell);
			}
			else {
				snake[cell].oldDir = snake[cell].newDir;
				moveSnake(snake[cell].newDir, snake, cell);
			}
		}
		else {
			snake[cell].oldDir = snake[cell].newDir;
			snake[cell].newDir = snake[cell - 1].oldDir;
			moveSnake(snake[cell].newDir, snake, cell);
		}
	}
	return snake[0].newDir;
}
//DIRECTIONS
void goRight(coordinate* snake, int arrCell)
{
	int i = arrCell;
	gotoxy(snake[i].col + 1, snake[i].row);
	printf("@");
	gotoxy(snake[i].col, snake[i].row);
	printf(" ");
	snake[i].col += 1;
}
void goLeft(coordinate* snake, int arrCell)
{
	int i = arrCell;
	gotoxy(snake[i].col - 1, snake[i].row);
	printf("@");
	gotoxy(snake[i].col, snake[i].row);
	printf(" ");
	snake[i].col -= 1;
}
void goDown(coordinate* snake, int arrCell)
{
	int i = arrCell;
	gotoxy(snake[i].col, snake[i].row + 1);
	printf("@");
	gotoxy(snake[i].col, snake[i].row);
	printf(" ");
	snake[i].row += 1;

}
void goUp(coordinate* snake, int arrCell)
{
	int i = arrCell;
	gotoxy(snake[i].col, snake[i].row - 1);
	printf("@");
	gotoxy(snake[i].col, snake[i].row);
	printf(" ");
	snake[i].row -= 1;
}

void food(coordinate* snake, int snakeSize, int* foodRow, int* foodCol)
{//the rendom num is for the coordinates of the food
 //getting a location on the board and prints food on screen
	int i;
	srand(time(NULL));
	*foodRow = rand() % 23 + 1;
	*foodCol = rand() % 73 + 1;

	for (i = 0; i < snakeSize; i++)
	{
		if (snake[i].row == *foodRow && snake[i].col == *foodCol) {
			*foodRow = rand() % 23 + 1;
			*foodCol = rand() % 73 + 1;
		}
	}
	gotoxy(*foodCol, *foodRow);
	printf("$");
}
coordinate* widenedSnake(coordinate* snake, int snakeSize)
{//func that widende the array to the new size 
	coordinate* newSnake;
	newSnake = (coordinate*)realloc(snake, snakeSize * sizeof(coordinate));

	if (newSnake == NULL) {
		printf("ERROR - could not reallocate");
		exit(1); //exit the program
	}
	else
		return newSnake;
}
int snakeEats(coordinate* snake, int snakeSize, int* foodRow, int* foodCol)
{//when the snake eats food this func get him bigger and return the new snake size
	snakeSize += 1;
	snake = widenedSnake(snake, snakeSize);
	snake[snakeSize - 1].newDir = snake[snakeSize - 2].oldDir;
	snake[snakeSize - 1].oldDir = snake[snakeSize - 2].oldDir;
	newCellInIt(snake, snakeSize);
	food(snake, snakeSize, foodRow, foodCol);

	return snakeSize;
}
void newCellInIt(coordinate* snake, int snakeSize)
{//after added a new cell to the array this func enter its values depending on the snake direction
	if (snake[snakeSize - 1].newDir == RIGHT) {
		snake[snakeSize - 1].row = snake[snakeSize - 2].row;
		snake[snakeSize - 1].col = snake[snakeSize - 2].col;
		snake[snakeSize - 1].col--;
	}
	else if (snake[snakeSize - 1].newDir == LEFT) {
		snake[snakeSize - 1].row = snake[snakeSize - 2].row;
		snake[snakeSize - 1].col = snake[snakeSize - 2].col;
		snake[snakeSize - 1].col++;
	}
	else if (snake[snakeSize - 1].newDir == DOWN) {
		snake[snakeSize - 1].row = snake[snakeSize - 2].row;
		snake[snakeSize - 1].col = snake[snakeSize - 2].col;
		snake[snakeSize - 1].row--;
	}
	else if (snake[snakeSize - 1].newDir == UP) {
		snake[snakeSize - 1].row = snake[snakeSize - 2].row;
		snake[snakeSize - 1].col = snake[snakeSize - 2].col;
		snake[snakeSize - 1].row++;
	}
}
bool hitBorder(coordinate* snake)
{// if the snake get to the borders of the board the func returns true else returns false
	if (snake[0].row == 0)
		return true;
	else if (snake[0].row == 25)
		return true;
	else if (snake[0].col == 0)
		return true;
	else if (snake[0].col == 75)
		return true;
	else
		return false;
}

bool goToParallelDir(coordinate* snake)
{// if the player try to go in the parallel direction (exmple: righ to left) the player lose and the func return true else return false
	if (snake[0].oldDir == RIGHT && snake[0].newDir == LEFT)
		return true;
	else if (snake[0].oldDir == LEFT && snake[0].newDir == RIGHT)
		return true;
	else if (snake[0].oldDir == DOWN && snake[0].newDir == UP)
		return true;
	else if (snake[0].oldDir == UP && snake[0].newDir == DOWN)
		return true;
	else
		return false;
}
bool hitItSelf(coordinate* snake, int snakeSize)
{// if the snake hits a part of its body the func returns true else false
	int i;
	for (i = 2; i < snakeSize; i++) // cant hit the cell close to the head
	{
		if (snake[0].row == snake[i].row && snake[0].col == snake[i].col)
			return true;
	}
	return false;
}
void winGame()
{// func prints a win 
	int i;
	system("cls");
	gotoxy(0, 0);
	for (i = 0; i < 20; i++) {
		printf("#");
	}
	gotoxy(5, 1);
	printf("WIN GAME!\n");
	for (i = 0; i < 20; i++) {
		printf("#");
	}
}

void loseGame()
{//func prints a lose
	int i;
	system("cls");
	gotoxy(0, 0);
	for (i = 0; i < 20; i++) {
		printf("#");
	}
	gotoxy(5, 1);
	printf("GAME OVER!\n");
	for (i = 0; i < 20; i++) {
		printf("#");
	}

}
void printFrame()
{// func prints the borders of the game board
	int i;
	for (i = 0; i <= 75; i++)
	{
		gotoxy(i, 0);
		printf("#");
		gotoxy(i, 25);
		printf("#");
	}
	for (i = 0; i <= 25; i++)
	{
		gotoxy(0, i);
		printf("#");
		gotoxy(75, i);
		printf("#");
	}
}
void printMenu()
{// func prints the menu (the options of games)
	int i;

	for (i = 0; i < 50; i++) {
		printf("*");
	}printf("\n");
	printf("* Pick Your Level, to exit press 0\n* 0 - Exit\n* 1 - Basic Level\n* 2 - Intermidiate Level\n* 3 - Advanced Level\n");
	//for (i = 0; i < 7; i++){
	//	//gotoxy(50, i);
	//	printf("*");
	for (i = 0; i < 50; i++) {
		printf("*");
	}printf("\n");
}
int levelOfGame()
{// when the player chooses a level of a game, the func get the choise and return the game level
	int gameLevel;
	printMenu(); // need to return the speed of the game 
	scanf("%d", &gameLevel);
	switch (gameLevel)
	{
	case 0: // exit
		exit(0);
		break;
	case 1: // basic level= the game 
		return gameLevel;
		break;
	case 2:
		system("cls");
		printMenu();
		return gameLevel;
		break;
	case 3:
		system("cls");
		printMenu();
		return gameLevel;
		break;
	default: // any key press
		system("cls");
		printMenu();
		return gameLevel;
		break;
	}
}