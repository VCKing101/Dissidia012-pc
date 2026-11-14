#include "menu/agent6_types.h"

class A6Screen : public A6Big {
public:
    char pad_04[0xDC];
    char field_E0[2][0x88];
};

extern "C" {
void func_08A52E4C(char *p);
void func_08A53C08(A6Screen *s);
void func_08A53B1C(A6Screen *s);

void func_08A5352C(A6Screen *self) {
    int i = 0;
    char *e = self->field_E0[0];
    do {
        func_08A52E4C(e);
        i++;
        e += 0x88;
    } while (i < 2);
    func_08A53C08(self);
    func_08A53B1C(self);
    self->s14();
}
}
