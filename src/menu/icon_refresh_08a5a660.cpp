/* Runs virtual step 8, reloads the four icon frames and registers them. */
#include "menu/agent12_types.h"

class FrameRefreshPart : public A12Part24 {
public:
    char pad_028[0x38 - 0x28];
    void *field_38;
    void *field_3C;
    void *field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
};

extern "C" int func_088DAA68(void *icon);
extern "C" void func_088D3CF0(int zero, int *frames, void *table);
extern "C" char D_08A5B6D0[];

extern "C" {
void func_08A5A660(FrameRefreshPart *self)
{
    self->v8();
    self->field_44 = func_088DAA68(self->field_3C);
    self->field_48 = func_088DAA68(self->field_3C);
    self->field_4C = func_088DAA68(self->field_40);
    self->field_50 = func_088DAA68(self->field_38);
    func_088D3CF0(0, &self->field_44, D_08A5B6D0);
}
}
