#include "menu/agent29_types.h"

/* Screen0B6A0: marks card `id` as seen in the save (both tables of
   D_089D712C +0x1E208 and the seen bitmap in D_089D726C) and refreshes
   the score flags. */
typedef struct SeenCard29 {
    char pad_00[0xC];
    short bit;
} SeenCard29;

extern unsigned int *D_089D726C;
extern void func_088F2BDC(void *table);
extern void func_088F3E08(void *table, unsigned short id);
extern SeenCard29 *func_089FCA28(Screen0B6A0 *self, unsigned short id);
extern void func_089FCF10(Screen0B6A0 *self);

void func_089FCE5C(Screen0B6A0 *self, short id)
{
    unsigned int bit;

    func_088F2BDC((char *)D_089D712C + 0x1E208);
    func_088F3E08((char *)D_089D712C + 0x1E208, id);
    bit = func_089FCA28(self, id)->bit;
    if ((int)bit >= 0) {
        D_089D726C[bit >> 5] |= 1 << (bit & 0x1F);
    }
    func_089FCF10(self);
}
