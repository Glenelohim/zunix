#include <stdio.h>
#include <curses.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 15

/*
LINES 		 //== the row numbers of terminal
COLS 		 //== the column numbers of terminal
initscr();	 //init the libcurses & tty
endwin();	 //close the libcurses & reset tty
clear();     //same as the command clear
move(x, y);  //move the cursor to the (x,y)
addstr(s);   //save the string "s" in the buffer of screen
addch(c);	 //save the character 'c' in the buffer of screen
refresh();   //refresh & display the buffer of screen
sleep(x);	 //make the program sleep x-second
getch();     //same as "press any key to continue"
standout();  //inverse the color of screen 
standend();  //shutdown the standout
*/

int main()
{
	const char *star = "O";
	const char *blank = " ";
	int direction = 1;  // -1=left, 1=right
	int pos = LEFTEDGE;
	initscr();
	clear();

	while (1)
	{
		move(ROW, pos);
		addstr(star);
		move(LINES-1, COLS-1);
		refresh();
		sleep(1);
		move(ROW, pos);
		addstr(blank);
		pos += direction;
		if (pos >= RIGHTEDGE)
			direction = -1;
		if (pos <= LEFTEDGE)
			direction = 1;
	}
	
	endwin();
	return 0;
}