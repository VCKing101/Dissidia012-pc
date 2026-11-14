#include "menu/agent12_types.h"

struct GridCell {
    char data[0x54];
};

class GridPanel : public A12Part24 {
public:
    char pad_028[0x50 - 0x28];
    char field_50[0x58];
    GridCell field_A8[16];
};

extern "C" void func_08A60408(char *list);
extern "C" void func_08A5C6B4(GridCell *cell);

extern "C" {
void func_08A5CF38(GridPanel *self)
{
    func_08A60408(self->field_50);
    for (int i = 0; i < 16; i++) {
        func_08A5C6B4(&self->field_A8[i]);
    }
    self->v9();
    self->v8();
}
}
