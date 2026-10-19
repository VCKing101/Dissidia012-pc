#ifndef MENU_SAVEDATA_DIALOG_H
#define MENU_SAVEDATA_DIALOG_H

#include "menu/execution_state.h"

/* Pinned main import-library/NID records bind these addresses to
 * sceUtilitySavedataGetStatus, ShutdownStart and Update respectively. */
extern int func_089C5868(void);
extern int func_089C5878(void);
extern void func_089C5898(int mode);

#endif
