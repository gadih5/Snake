// ID: 206897639
// full name: Gadi Haik

//******************************************************************************************************************//
//                                                   ~Snake~														//
//																													//
// This program is the game "Snake".																				//
// At the beggining of the game the snake (that is represented by '@') appears in the center of the game's board,	//
// which is the size of 25 rows on 75 columns (that is represented by '#').											//
// The snake starts to move to the right side and is always in movement.											//
// The initial movement speed is a one second per step,																//	
// and it increases by 0.03 seconds every fifth step, until the maximum speed of 0.1 seconds per step.				//
// The initial lenght of the snake is one link, and it grows by one link every fifth step.							//
// The user controls the snake's movement direction by pressing one of the following keys:							//
// 'd' for right, 'a' for left, 'w' for up and 's' for down.														//
// The game is keep running until the snake's head touchs one of the rest of the snake's links,						//
// or if the snake's head touchs the game's board.																	//
// When that happaned a "GAME OVER" announcment will appear, and the game is over.									//
//																													//
//******************************************************************************************************************//

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulatoion).

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);

typedef struct snakeLink
{
	char c;					 /// The character of the snake's link.
	int x;					 /// The row of the snake's link.
	int y;					 /// The column of the snake's link.
	struct snakeLink* next;  /// The next snake's link.
}SnakeLink;

/* This function recives a row, a column and a character, and creates a new snake's link
   with those values ,locates it as the last link and returns the new snake's link. */
SnakeLink* createNewSnakeLink(int x, int y, char c);

/* This function recives the current head of the snake, it's row and column. The function creates
   a new snake's link and replaces the old head with the new created link (the new head) of the snake. */
SnakeLink* changeToNewHead(SnakeLink* head, int x, int y);

/* This function moves the curser to the left top corner of the screen and prints a frame of 25 rows on 75 columns (that is represented by "#"). */
void printBoard();

/* This function recives a SnakeLink, and frees every SnakeLink of it's linked list. */
void freeSnake(SnakeLink* head);

/* This function recives a SnakeLink, and disconnects the last SnakeLink from the linked list. */
void keepSnakeLenght(SnakeLink* head);

/* This function recives a SnakeLink, and prints the character of the SnakeLink at it's location.
   The function continues to print the character of a SnakeLink at every location of a SnakeLink, until the last SnakeLink. */
void printSnake(SnakeLink* head);

/* This function recives an address of real number and an integer, if the integer equels to five and the real number is greater than 0.1,
   the function will decrease 0.03 from the real number and will update the new value at the address. */
void changeSleepTime(double* sleepTime, int counter);

/* This function recives a SnakeLink, and prints a space (' ') at the location of the SnakeLink 
   and continues to print a space at every location of a SnakeLink, until the last SnakeLink. */
void deleteSnake(SnakeLink* head);

/* This function prints the game's board and a "GAME OVER" announcment in the middle of the game's board. */
void gameOverScreen();

/* This function recives a SnakeLink, and returns if the SnakeLink touched any of the rest of the previous links. */
bool gameOverFromSelf(SnakeLink* head);

/* This function recives a SnakeLink, and returns if the SnakeLink touched the game's board. */
bool gameOverFromBoard(SnakeLink* head);

#define ROWS 25
#define COLS 75

int main()
{
	bool gameOver = false;			/// Initializes a variable that the game can run until it changes.
	double sleepTime = 1;			/// Initializes the initial movement speed to one second.
	int stepCounter = 1;			/// Initializes a variable that counts every step of the snake.
	int headCurrentRow = ROWS / 2;	/// Initializes a variable that will contain the current row that the snake's head is in.
	int headCurrentCol = COLS / 2;	/// Initializes a variable that will contain the current column that the snake's head is in.
	char c = 'd', prevC = 'd';		/// Initializes the initial movement direction (right),
	                                /// and initializes another variable that contains the previous movement direction.
	SnakeLink* head = createNewSnakeLink(headCurrentRow, headCurrentCol, '@'); /// Initializes the head of the snake.
	printBoard();                   /// Prints the game's board.
	while (gameOver == false)       /// Until the snake's head touches another link or the game's board, the game will keep running.
	{
		printSnake(head);							/// Prints all the links of the snake.
		changeSleepTime(&sleepTime, stepCounter);	/// Increases the speed of the snake's movement every five steps.
		sleep(sleepTime);							/// Delays the game according to the snake's movement speed.
		deleteSnake(head);						    /// Deletes the unwanted snake's links, according to the snake's lenght.
		if (_kbhit())								/// Checks if the user clicked on any key:
			c = _getch();							/// Saves the character of the key that the user has clicked on.

		if (c != 'd' && c != 'a' && c != 'w' && c != 's') /// The key is not one of the movement keys:
			c = prevC;								/// Saves the previous movement key, so the snake will continue to move in the same direction.

		switch (c)									
		{
		case 'd':									/// The right direction:
			headCurrentCol++;						/// Updates the current column of the snake's head to the next column.
			break;					
		case 'a':									/// The left direction:
			headCurrentCol--;						/// Updates the current column of the snake's head to the previous column.
			break;
		case 'w':									/// The up direction:
			headCurrentRow--;						/// Updates the current row of the snake's head to the next row.
			break;
		case 's':									/// The down direction:
			headCurrentRow++;						/// Updates the current row of the snake's head to the previous row.
			break;
		}

		prevC = c; /// Updates the previous direction.
		
		head = changeToNewHead(head, headCurrentRow, headCurrentCol); /// Updates the head to the new head, according to the new location.

		if (gameOverFromBoard(head) || gameOverFromSelf(head))		  /// Checks if the snake's head touched another link or the game's board:
		{
			gameOver = true;									   	  /// The loop will stop in the next iteration.
		}
		if (stepCounter <= 5)									      /// Not a fifth step:
		{
			keepSnakeLenght(head);									  /// The snake still remains the same lenght.
		}
		else                                                          /// Every fifth step:
		{
			stepCounter = 0;										  /// Resets the step counter.
		}
		stepCounter++;											      /// Updates the step counter.
	}

	gameOverScreen();	/// The game is over, prints the game's board with "GAME OVER" announcement.
	freeSnake(head);	/// Frees the snake related list.
}

// Turn escape codes ON.
void init(void) {
	system("cls");
}

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs*CLOCKS_PER_SEC);
}

SnakeLink* createNewSnakeLink(int x, int y, char c)
{
	SnakeLink* link = (SnakeLink*)malloc(sizeof(SnakeLink)); /// Creates a snake link and allocates a memory for it.
	link->c = c;        /// Updates the link's character.
	link->x = x;		/// Updates the link's row.
	link->y = y;		/// Updates the link's column.
	link->next = NULL;	/// Updates the next link to NULL.

	return link;		/// Retuens the new link.
}

SnakeLink* changeToNewHead(SnakeLink* head, int x, int y)
{
	SnakeLink* NewHead = createNewSnakeLink(x, y,'@'); /// Creates a new link.
	NewHead->next = head;	/// Updates the head of the snake to be the new created link.
	return NewHead;			/// Returns the new head.
}

void printBoard()
{
	gotoxy(0, 0);						/// Moves the cursor to the left top corner of the screen. 
	int row, col;						

	for (col = 1; col <= COLS; col++)	/// Prints the top of the frame.
	{
		printf("#");
	}
	printf("\n");

	for (row = 2; row <= ROWS - 1; row++) /// Prints the right and left parts of the frame.
	{
		printf("#");
		for (col = 2; col <= COLS - 1; col++) 
		{
			printf(" ");
		}
		printf("#");
		printf("\n");
	}

	for (col = 1; col <= COLS; col++) /// Prints the bottom of the frame.
	{
		printf("#");
	}
	printf("\n");
}

void freeSnake(SnakeLink* head)
{
	SnakeLink* next;

	while (head != NULL)   /// Until the end of the snake.
	{
		next = head->next; /// Saves the link after the head.
		free(head);        /// Frees the head.
		head = next;       /// Updates the head to be the next link.
	}
}

void keepSnakeLenght(SnakeLink* head)
{
	while (head->next->next != NULL) /// Until the link that is before the last link.
	{
		head = head->next;           /// Updates the head to be the next link.
	}
	head->next = NULL;               /// Shortens the snake by one link.
}

void printSnake(SnakeLink* head)
{
	while (head != NULL)           /// Until the end of the snake.
	{
		gotoxy(head->x, head->y);  /// Moves the cursor to the head's location.
		printf("%c", head->c);     /// Prints the head's character.
		head = head->next;         /// Updates the head to be the next link.
	}
	gotoxy(ROWS + 1,1);            /// Moves the cursor to the outside of the game's board. 

}

void deleteSnake(SnakeLink* head)
{
	while (head != NULL)           /// Until the end of the snake.
	{
		gotoxy(head->x, head->y);  /// Moves the cursor to the head's location.
		printf(" ");               /// Prints space.
		head = head->next;         /// Updates the head to be the next link.
	}
}
void changeSleepTime(double* sleepTime, int counter)
{
	if (counter == 5 && *sleepTime > 0.1) /// Every fifth step and under a condition that the movement speed is lower than 0.1 seconds per step:
	{
		*sleepTime -= 0.03;               /// Increases the movement speed of the snake by 0.03 seconds.
	}
}

bool gameOverFromSelf(SnakeLink* head)
{
	int x = head->x;       /// Saves the current head's row.
	int y = head->y;       /// Saves the current head's column.
	head = head->next;     /// Changes the head to be the next link.
	while (head != NULL)   /// Until the end of the snake. 
	{
		if (x == head->x && y == head->y)  /// The original head and the current link locations are same.
			return true;   /// The snake touched itself.
		head = head->next; /// Updates the head to be the next link.
	}
	return false;          /// The snake didn't touch itself, the game continues.
}

bool gameOverFromBoard(SnakeLink* head)
{
	if (head->x == ROWS || head->x == 0 || head->y == COLS || head->y == 0) /// The snake's head and the game's board locations are same.
	{
		return true; /// The snake touched the game's board.
	}
	return false;    /// The snake's head didn't touch the game's board, the game continues.
}

void gameOverScreen()
{
	printBoard();					  /// Prints the game's board.
	gotoxy(ROWS / 2, (COLS / 2) - 4); /// Moves the cursor to the center of the game's board.
	printf("GAME OVER");			  /// Prints "GAME OVER" announcement.
	gotoxy(ROWS + 1, 1);			  /// Moves the cursor to the outside of the game's board. 
}
