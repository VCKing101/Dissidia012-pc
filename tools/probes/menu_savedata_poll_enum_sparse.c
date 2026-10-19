/* Historical nonmatching draft: status labels differ from the pinned polling
 * handlers. Use src/menu/savedata_list_poll.c and savedata_load_poll.c. */
/* Nonmatching exploration: not part of the C manifest and earns no source credit. */
#include "menu/savedata_dialog.h"

enum MenuDialogStatus {DialogNone=0,DialogVisible=2,DialogQuit=3,DialogFinished=4};
void func_089DD930(MenuRuntimePrefix *self) {
    switch ((enum MenuDialogStatus)func_089C5868()) {
    case 2:
        func_089C5898(2);
        break;
    case 3:
        func_089C5878();
        break;
    default:
        break;
    case 4:
        self->field_01DC->field_0008++;
        break;
    case 0:
        func_089C5878();
        self->field_01DC->field_0008 = 6;
        break;
    }
}
