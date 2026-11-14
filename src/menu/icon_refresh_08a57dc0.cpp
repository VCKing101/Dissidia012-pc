/* Runs virtual step 8, reloads the five icon frames and registers them. */
#include "menu/agent12_types.h"

class IconRefreshPart : public A12Part24 {
public:
    char pad_028[0x34 - 0x28];
    void *field_34;
    void *field_38;
    char pad_03C[0xC8 - 0x3C];
    void *field_C8;
    void *field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
};

extern "C" int func_088DAA68(void *icon);
extern "C" void func_088D3CF0(int zero, int *frames, void *table);
extern "C" char D_08A585FC[];

extern "C" {
void func_08A57DC0(IconRefreshPart *self)
{
    self->v8();
    self->field_D0 = func_088DAA68(self->field_C8);
    self->field_D4 = func_088DAA68(self->field_38);
    self->field_D8 = func_088DAA68(self->field_34);
    self->field_E0 = func_088DAA68(self->field_34);
    self->field_DC = func_088DAA68(self->field_CC);
    func_088D3CF0(0, &self->field_D0, D_08A585FC);
}
}
