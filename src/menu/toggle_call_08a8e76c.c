typedef struct A6TgSelf3 {
    char pad_00[8];
    char *field_08;
} A6TgSelf3;

void func_088D4FE4(char *p);
void func_088D50DC(char *p);

void func_08A8E76C(A6TgSelf3 *self, unsigned char on) {
    char *p = self->field_08 + 0xAE4;
    if (on == 1) {
        func_088D4FE4(p);
    } else {
        func_088D50DC(p);
    }
}
