#include "menu/agent36_types.h"

/* Finds item in the slot groups of every character in the save data and
 * returns a bit per character that has it (bit 0: character 1); with
 * remove set, those slots are cleared. */
unsigned int func_08A255AC(void *self, unsigned short item, unsigned char remove)
{
    unsigned int found = 0;
    int chara;
    int i;
    int k;

    for (chara = 1; chara < 33; chara++) {
        if (func_088CB1EC(D_089D712C->charas, chara) != 0) {
            SlotGroup36 *groups = func_088CB164(D_089D712C->charas, chara)->groups;
            for (i = 0; i < 5; i++) {
                SlotGroup36 *group = &groups[i];
                for (k = 0; k < 4; k++) {
                    if (func_088B7B18(group, k) == item) {
                        found |= 1 << (chara - 1);
                        if (remove) {
                            func_088B765C(group, k, -1);
                        }
                    }
                }
            }
        }
    }
    return found;
}
