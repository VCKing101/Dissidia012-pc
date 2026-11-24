/* Returns the word at offset 0x3F548 of the menu work block. */
#include "menu/agent49_types.h"
extern char *D_089D712C;
int func_08B451E4(void) { return ((struct W0 *)(D_089D712C + 0x3F548))->v; }
