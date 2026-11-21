#include "menu/agent36_types.h"

/* Battle setup: resets the table state, plays the opening layout and goes
 * on by the list mode (0: back from a battle, 4: replay, else state 4). */

extern ScriptCmd36 D_08A4533E[];
extern void *D_089D6C88;
void func_088A1878(void *object, float x, float y);
void func_088D3910(int kind, void *layout, ScriptCmd36 *script, int a, int b);
void func_08885A9C(void *object);
void func_08A41A9C(BattleGlobal36 *self);
void func_08A41AD4(BattleGlobal36 *self);

void func_08A4243C(BattleGlobal36 *self)
{
    int i;

    func_088A1878(D_089D6C88, 0.0f, 0.0f);
    self->table->flags = 0;
    self->table->field_129B = 0;
    self->table->field_1296 = 0;
    for (i = 0; i < 9; i++) {
        self->table->field_E70[i] = 0;
    }
    func_088D3910(0x11, self->table->layout, D_08A4533E, 0, 1);
    func_08885A9C(self->table->layout[0]);
    func_08885A9C(self->table->layout[1]);
    switch (D_09B6DA68.mode) {
    case 0:
        self->table->field_1299 = D_09B6DA68.field_20;
        func_08A41A9C(self);
        func_08A41818(self, 0);
        func_08A41AD4(self);
        self->table->state = 6;
        break;
    case 4:
        func_08A41A9C(self);
        func_08A41818(self, 4);
        func_08A41AD4(self);
        self->table->field_129C = 1;
        self->table->state = 6;
        break;
    default:
        self->table->state = 4;
        break;
    }
}
