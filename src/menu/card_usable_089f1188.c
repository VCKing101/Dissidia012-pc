#include "menu/agent29_types.h"

/* Screen0B6A0: whether card `id` can be offered: not owned yet, flagged
   in both +0x08 and +0x09 of its record, unlocked in the save and not
   already listed (func_089F1114). */
typedef struct Card6CA4 {
    int item;
    char pad_04[0x4];
    unsigned char field_08;
    unsigned char field_09;
} Card6CA4;

extern Card6CA4 *func_088C6CA4(int id);
extern int func_088B912C(void *owned, int item);
extern int func_088F4308(void *table, int id);
extern int func_089F1114(Screen0B6A0 *self, int id);

int func_089F1188(Screen0B6A0 *self, char id)
{
    int result = 0;
    Card6CA4 *card = func_088C6CA4(id);

    if (func_088B912C((char *)D_089D712C + 0xF6E0, card->item) == 0 && card->field_08 != 0 &&
        card->field_09 != 0 && func_088F4308((char *)D_089D712C + 0x1E208, id) != 0) {
        if (func_089F1114(self, id) == 0) {
            result = 1;
        }
    }
    return result;
}
