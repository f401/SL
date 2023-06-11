#ifndef __COLORS__
#define __COLORS__

#define sl_theme_hideCursor() printf("\33[?25l");
#define sl_theme_showCursor() printf("\33[?25h");

#define sl_theme_printRed() sl_theme_printClose();printf("\033[47;31m");
#define sl_theme_printBackGreener() sl_theme_printClose();printf("\033[46;37m");
#define sl_theme_printClose() printf("\033[0m");
#define sl_theme_printBackGreen() sl_theme_printClose();printf("\033[42;37m");
#define sl_theme_printBackGreenWithRed() sl_theme_printClose();printf("\033[42;31m");

#endif
