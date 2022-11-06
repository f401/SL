#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "screen/ncurses_main.h"
#include "screen/ncurses_theme.h"
#include "main.h"
#include "other_tools/tool.h"
#include "other_tools/log.h"

static void printw0Number(unsigned int number);
static char getChar4ncurses(int num, int debug);

static Vector2d getMousePosByXY(unsigned int x, unsigned int y);

static void printw0Number(unsigned int number) {
	register unsigned int contOfX = sl_tools_getNumberBit(X), countOfNumber = sl_tools_getNumberBit(number);
	register unsigned int needCount = contOfX - countOfNumber;
	if (needCount > 0) {
		char* zeros = sl_tools_getsign(needCount, '0');
		printw("%s", zeros);
		free(zeros);
	}

}

static Vector2d getMousePosByXY(unsigned int x, unsigned int y) {
	Vector2d ret = {-1, -1};
	ret.x = y;
	const int spa = SPACES + sl_tools_getNumberBit(X) - 1;
	for (register unsigned int i = 1; i < X + 1; i++) {
		register int nowX = spa * i + i;//4, 8, 12, 16
		register int leftX = (i - 1) * spa + i - 1;//0, 4, 8, 12
		if (leftX <= x && x <= nowX) {
			register int center = leftX + (spa / 2);
			ret.y = x < center ? i - 1 : i;
			break;
		}
	}
	return ret;
}

void sl_ncursesMain_initNcurses() {
	sl_logger_info("Init ncurses, code:%d", 0);
	
	initscr();
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	raw();
	start_color();
	sl_ncursesTheme_initPairs();
	
}

static char getChar4ncurses(int num, int debug) {
	switch(num) {
		case WHO_MINE:
			if (debug) {
				return MINE;
			}
			return FLAG;
		case WHO_EMPTY:return EMPTY;
	}
	return ' ';
}

UserInput sl_ncursesMain_runSecondMouse() {
	int ch = getch();

	UserInput v; 
	static UserInput lastInput = {SIMPLE_NOTHING, {-1, -1}, ""};
	v.pos.x = -1;
	v.pos.y = -1;
	v.status = -1;

	if(ch == KEY_MOUSE) {
		MEVENT event;
		if (getmouse(&event) == OK) {
			sl_logger_info("x: %d, y: %d, event: %d", event.x, event.y, event.bstate);
			if (event.bstate == BUTTON1_CLICKED || event.bstate == BUTTON3_CLICKED || event.bstate == BUTTON1_DOUBLE_CLICKED) {
			
				v.pos = getMousePosByXY(event.x, event.y);
				sl_logger_info("v.x: %d, v.y: %d", v.pos.x, v.pos.y);

				v.status = SIMPLE_NOTHING;

				if (v.pos.x == lastInput.pos.x && v.pos.y == lastInput.pos.y) {
					v.status = event.bstate == BUTTON1_CLICKED ? SIMPLE_OPEN_MODE : SIMPLE_SET_FLAG_MODE;
				} else if (event.bstate == BUTTON1_DOUBLE_CLICKED) {
					v.status = SIMPLE_OPEN_MODE;
				}

			} 
		}
	} else if (ch == KEY_F(1)) {
		sl_main_exitGame(SAVE_STATUS_SAVE_STATUS);
	}

	lastInput = v;

	return v;
}

void sl_ncursesMain_printData(Vector2d pos, int debug) {	
	char* space = sl_tools_getsign(SPACES, ' ');
	{
		sl_ncursesTheme_setAttrToRed();
		for (register unsigned int i = 0; i < X; i++) {
			if (i == 0) {
				char* fixSpace = sl_tools_getsign(sl_tools_getNumberBit(X) + SPACES, ' ');
				printw("%s", fixSpace);
				free(fixSpace);
			} else {
				printw("%s", space);
			}
			if (i == pos.y) {
				sl_ncursesTheme_setAttrToBackGreenWithRed();
			}
			printw0Number(i + 1);
			printw("%d", i + 1);
			sl_ncursesTheme_setAttrToRed();
		}
		sl_ncursesTheme_setAttrToNull();
	}

	printw("\n");

	{
		char* spa = sl_tools_getsign(sl_tools_getNumberBit(X) - 1, ' ');

		bool isHighLine = false;

		for(register unsigned int w = 0;w < X;w++) {
			if(w == pos.x) {
				sl_ncursesTheme_setAttrToBackGreenWithRed();
				isHighLine = true;
			} else {
				sl_ncursesTheme_setAttrToRed();
			}
			printw0Number(w + 1);
			printw("%d", w + 1);
			sl_ncursesTheme_setAttrToNull();
			for(register unsigned int h = 0; h < X; h ++) {
				Block dat = *(gameData + w * X + h);//read only
				if (isHighLine) 
					sl_ncursesTheme_setAttrToBackGreen();
				printw("%s", space);
				register int who = dat.who, status = dat.status;
				if(h == pos.y) {
					sl_ncursesTheme_setAttrToBackGreen();
					
					if(isHighLine)
						sl_ncursesTheme_setAttrToBackGreener();
				}

				if(debug) {
					if (who > 0) {
						printw("%s%d", spa, who);
					} else {
						printw("%s%c", spa, getChar4ncurses(who, 1));
					}

				} else {
					if (status == STATUS_OPEN && who > 0) {// is number and opening
						printw("%d%s", who, spa);
					} else if (status == STATUS_CLOSE) {// is close
						printw("%c%s", UNKNOW, spa);
					} else if (status == STATUS_FLAG) {
						printw("F%s", spa);
					} else {// other
						printw("%c%s", getChar4ncurses(who, 0), spa);
					}

				}
				sl_ncursesTheme_setAttrToNull();
			}	
			sl_ncursesTheme_setAttrToNull();
			isHighLine = false;
			printw("\n");
		}
		free(spa);

	}
	sl_ncursesTheme_setAttrToNull();
	free(space);
	refresh();
}

