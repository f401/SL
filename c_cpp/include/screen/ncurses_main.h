#ifndef __NCURSES_MAIN_H__
#define __NCURSES_MAIN_H__ 1

#include "main.h"

void sl_ncursesMain_initNcurses();
#define sl_ncursesMain_clearNcursesScreen() clear();
#define sl_ncursesMain_exitNcurses() endwin();
void sl_ncursesMain_printData(Vector2d pos, int debug);
UserInput sl_ncursesMain_runSecondMouse();
#endif
