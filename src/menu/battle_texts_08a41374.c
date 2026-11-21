#include "menu/agent36_types.h"

/* Battle setup: makes the table's texts (from D_08A455C0) and their
 * sprites, one pair per text id. */

typedef struct TextRow36 {
    unsigned short text;
    char pad_02[0x4];
} TextRow36;

extern TextRow36 D_08A455C0[];

void func_08A41374(BattleGlobal36 *self)
{
    int i;

    for (i = 0; i < self->table->text_count; i++) {
        int pair = i * 2;
        self->table->texts[8 + pair] = func_08A412A8(self, D_08A455C0[self->table->text_ids[i]].text, 0, 0);
        self->table->texts[8 + pair + 1] = func_0887E534(self->table->texts[8 + pair]);
    }
}
