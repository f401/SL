#include "screen/ncurses_theme.h"
#include <ncurses.h>
#include <stdlib.h>
#include "other_tools/log.h"

static volatile int who = 0;

void sl_ncursesTheme_initPairs() {
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_GREEN);
}

void sl_ncursesTheme_setAttrToBackGreen() {
	//info("setting attr to back green");
	sl_ncursesTheme_setAttrToNull();
	attron(COLOR_PAIR(3));
	who = 3;
}

void sl_ncursesTheme_setAttrToBackGreenWithRed() {
	//info("setting attr to back green with red");
	sl_ncursesTheme_setAttrToNull();
	attron(COLOR_PAIR(4));
	who = 4;
}

void sl_ncursesTheme_setAttrToBackGreener() {
	//info("setting attr to back blue");
	sl_ncursesTheme_setAttrToNull();
	attron(COLOR_PAIR(2));
	who = 2;
}

void sl_ncursesTheme_setAttrToRed() {
	//info("setting attr to red");
	sl_ncursesTheme_setAttrToNull();
	attron(COLOR_PAIR(1));
	who = 1;
}

void sl_ncursesTheme_setAttrToNull() {
	if (who != 0) {
		//info("clearing attr");
		attroff(COLOR_PAIR(who));
	}
	who = 0;
}

