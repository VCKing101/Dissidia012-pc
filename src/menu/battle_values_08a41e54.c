#include "menu/agent36_types.h"

/* Battle setup: remakes the texts and sprites (slots 0x1A..0x1F) of the
 * three shown values. */

extern const char D_08A45648[];

void func_08A41E54(BattleGlobal36 *self)
{
    void **texts = self->table->texts;
    char *lines[1];
    int one;
    int i;

    for (i = 0x1A; i < 0x20; i++) {
        if (texts[i] != 0) {
            func_0887BA5C(texts[i]);
            texts[i] = 0;
        }
    }
    lines[0] = self->table->buffer;
    one = 1;
    for (i = 0; i < 3; i++) {
        BattleValue36 *value = &self->table->shown[i];
        int pair = i * 2;
        TextObj36 *text;
        func_089347E8(self->table->buffer, D_08A45648, value->value);
        text = func_08A412A8(self, value->kind, lines, one);
        texts[0x1A + pair] = text;
        texts[0x1A + pair + 1] = func_0887E534(text);
        ((TextObj36 *)texts[0x1A + pair + 1])->flags |= 0x800;
    }
}
