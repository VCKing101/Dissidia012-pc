#include "menu/agent36_types.h"

/* Battle setup: closes the table: frees its texts and messages, closes the
 * message box and goes on to state 2 (saving first) or 3. */

void func_088D3A04(void *layout);
void func_0887B1F4(void *messages, int arg);
void func_08885220(void *box);
void func_088AC8C8(Save36 *save, int arg);

void func_08A4258C(BattleGlobal36 *self)
{
    int i;

    func_088D3A04(self->table->layout);
    for (i = 0; i < 0x24; i++) {
        if (self->table->texts[i] != 0) {
            func_0887BA5C(self->table->texts[i]);
            self->table->texts[i] = 0;
        }
    }
    if (self->table->messages != 0) {
        func_0887B1F4(self->table->messages, 3);
        self->table->messages = 0;
    }
    func_08885220(D_089D69E8);
    if (self->table->field_1298 != 0 && D_09B6DA68.field_1E == 0) {
        func_088AC8C8(D_089D712C, 1);
        self->table->state = 2;
    } else {
        self->table->state = 3;
    }
}
