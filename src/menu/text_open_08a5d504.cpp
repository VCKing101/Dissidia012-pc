/* Runs virtual step 9, opens the text resource and binds it to the label. */
#include "menu/agent12_types.h"

class TextOpenPart : public A12Part24 {
public:
    char pad_028[0x3C - 0x28];
    char field_3C[0x10];
    int field_4C;
};

extern "C" int func_0887B3EC(void *source, int kind, int a, int b, int c, int d);
extern "C" void func_088D428C(void *label, int *handle, void *table, int one);
extern "C" char D_08A5DC28[];

extern "C" {
void func_08A5D504(TextOpenPart *self, void *source)
{
    self->v9();
    self->field_4C = func_0887B3EC(source, 8, 0, 0x43, 0, 0);
    func_088D428C(self->field_3C, &self->field_4C, D_08A5DC28, 1);
}
}
