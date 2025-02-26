#include <curses.h>
#include <string.h>
#include <stdlib.h>

int colourkey_to_pair(char);

WINDOW *win;
int height, width;
int cutoff;
int count = 1;

void reset_screen(){
	wclear(win);
	box(win, 0, 0);

	mvwprintw(win, 0, 2, "Chat");
	mvwprintw(win, 1, 1, "Press 'q' or 'Q' to quit.");

	mvwprintw(win, height-3, 2, "+");
}

void display_start(void){
	initscr();
	curs_set(1);

	// colours
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

	getmaxyx(stdscr, height, width);
	cutoff = height - 6;

	win = newwin(height, width, 0, 0);

	reset_screen();
}

void display_rcv(char *message){
	// TODO send the size over
	char msg[1024 + 32 + 2];
	strcpy(msg, message);

	// check if we've reached the end of the screen.
	if(count > cutoff){
		// TODO scrolling
		reset_screen();
		count = 1;
	}
	
	int column = count + 1;
	
	/* split into tokens and do colour stuff. */
	//mvwprintw(win, column, 1, message);
	wmove(win, column, 1);
	
	char *token = strtok(msg, " ");

	wprintw(win, "%s ", token);
	
	while(token != NULL){
		token = strtok(NULL, " ");
		if(token == NULL)
			break;

		if(strlen(token) == 2 && token[0] == '@'){
			wattron(win, COLOR_PAIR(colourkey_to_pair(token[1])));
			token = strtok(NULL, " ");
			if(token == NULL)
				break;
		}

		wprintw(win, "%s ", token);

		wattrset(win, A_NORMAL);
	}


	// new line
	count++;

	wmove(win, height-3, 3);
	wrefresh(win);
}

void display_input(char *str, int len){
	wgetnstr(win, str, len);
}

void display_end(void){
	endwin();

	system("cls");
}

int colourkey_to_pair(char colour){
	switch(colour){
		case 'r':
			return 2;
		case 'g':
			return 3;
		case 'b':
			return 4;
		case 'y':
			return 5;
		case 'c':
			return 6;
		case 'm':
			return 7;
		default:
			return 1;
	}
}
