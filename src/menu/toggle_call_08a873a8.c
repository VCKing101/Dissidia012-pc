typedef struct A6TogSelf {
    char pad_00[0x14C];
    char *field_14C;
} A6TogSelf;

void func_088D4FE4(char *p);
void func_088D50DC(char *p);

void func_08A873A8(A6TogSelf *self, unsigned char on) {
    char *p = self->field_14C + 0xAA4;
    if (on == 1) {
        func_088D4FE4(p);
    } else {
        func_088D50DC(p);
    }
}
