/* Clears the three rows, the five panels and the cursor, runs virtual steps 9 and 8. */
#include "menu/agent12_types.h"

struct ClearRow {
    char data[0x68];
};

struct ClearPanel {
    char data[0x4C4];
};

class ClearPart : public A12Part24 {
public:
    char pad_028[0x2C - 0x28];
    int field_2C;
    char pad_030[0x78 - 0x30];
    ClearRow field_78[3];
    ClearPanel field_1B0[5];
    char pad_1984_unused[0x1984 - 0x1B0 - 5 * 0x4C4];
    char field_1984[0x10];
};

extern "C" void func_08A58988(ClearRow *row);
extern "C" void func_08A59158(ClearPanel *panel);
extern "C" void func_08A60408(void *cursor);

extern "C" {
void func_08A5A02C(ClearPart *self)
{
    int i;
    for (i = 0; i < 3; i++) {
        func_08A58988(&self->field_78[i]);
    }
    for (i = 0; i < 5; i++) {
        func_08A59158(&self->field_1B0[i]);
    }
    func_08A60408(self->field_1984);
    self->v9();
    self->v8();
    self->field_2C = 0;
}
}
