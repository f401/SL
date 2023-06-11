#include "ter/consle.h"
#include "main.h"
#include "other_tools/tool.h"
#include "ter/process.h"
#include "ter/theme.h"
#include <other_tools/log.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char getCharByDebug(Who who);
// print others
static void printNumZero(unsigned int num);
static inline void printTopLine(Vector2d pos, const char *space);

int sl_process_showProcess(unsigned int pro, unsigned int max,
                           const char *message) {
  const unsigned int cou = 20;
  sl_theme_hideCursor();
  if (max == 0) {
    return -1;
  }
  float p = (float)pro / (float)max;

  int value1 = p * cou;
  int value = cou - value1;

  char *va = sl_tools_getsign(value, '.');
  char *va1 = sl_tools_getsign(value1, '#');
  if (p == 1.0) {
    printf("\r%s: 100\%  [%s] %d/%d ...........Complete!\n", message, va1, pro,
           max);
    sl_theme_showCursor();
  } else {
    printf("\r%s: %d\% \[%s%s] %d/%d", message, (int)(p * 100), va1, va, pro,
           max);
  }
  free(va);
  free(va1);
  return 0;
}

void sl_consle_printDataForDEBUG(Vector2d pos, int DEBUG) {
  char *space = sl_tools_getsign(SPACES, ' ');
  char *spac = sl_tools_getsign((sl_tools_getNumberBit(X) - 1), ' ');
  printTopLine(pos, space);
  bool isHighLine = false;
  for (register unsigned int w = 0; w < X; w++) {
    if (w == pos.x) {
      sl_theme_printBackGreenWithRed();
      isHighLine = true;
    } else {
      sl_theme_printRed();
    }
    printNumZero(w + 1);
    printf("%d", w + 1);

    sl_theme_printClose();

    for (register unsigned int h = 0; h < X; h++) {
      Block dat = *(gameData + w * X + h);
      if (h == pos.y && isHighLine) {
        sl_theme_printBackGreen();
        printf("%s", space);
        sl_theme_printBackGreener();
        printf("%s", spac);
      } else if (isHighLine || h == pos.y) {
        if (isHighLine) {
          sl_theme_printBackGreen();
          printf("%s%s", space, spac);
        } else {
          printf("%s", space);
          sl_theme_printBackGreen();
          printf("%s", spac);
        }
      } else {
        printf("%s%s", space, spac);
      }
      register int who = dat.who, status = dat.status;
      if (DEBUG) {
        printf("%c", getCharByDebug(who));
      } else {
        if (status == STATUS_OPEN && who > 0) {
          printf("%d", who);
        } else if (status == STATUS_OPEN && who == 0) {
          printf("%c", EMPTY);
        } else if (status == STATUS_CLOSE) {
          printf("%c", UNKNOW);
        } else if (status == STATUS_FLAG) {
          printf("%c", FLAG);
        } else {
          sl_logger_error("Unknow who %d", who);
        }
      }
      sl_theme_printClose();
    }
    sl_theme_printClose();
    isHighLine = 0;
    printf("\n");
  }
  sl_theme_printClose();
  free(space);
  free(spac);
}

static inline void printTopLine(Vector2d pos, const char *space) {
  sl_theme_printRed();
  char *spa = sl_tools_getsign(sl_tools_getNumberBit(X), ' ');
  printf("%s", spa);
  free(spa);
  for (register unsigned int i = 0; i < X; i++) {
    register int temp = i + 1;
    printf("%s", space);
    if (i == pos.y) {
      sl_theme_printBackGreenWithRed();
    }
    printNumZero(temp);
    printf("%d", temp);
    sl_theme_printRed();
  }
  sl_theme_printClose();
  printf("\n");
}

inline static char getCharByDebug(Who who) {
  if (who > 0) {
    return (char)who + 48;
  } else {
    switch (who) {
    case WHO_MINE:
      return MINE;
    case WHO_EMPTY:
      return EMPTY;
    }
  }
  return ' ';
}

inline static void printNumZero(unsigned int num) {
  unsigned int c = sl_tools_getNumberBit(X) - sl_tools_getNumberBit(num);
  if (c > 0) {
    char *ch = sl_tools_getsign(c, '0');
    printf("%s", ch);
    free(ch);
  }
}
