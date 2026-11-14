typedef struct A6TgInner2 {
    char pad_000[0x254];
    char *field_254;
} A6TgInner2;

typedef struct A6TgSelf2 {
    char pad_00[8];
    A6TgInner2 *field_08;
} A6TgSelf2;

void func_0887B924(char *p);
void func_0887B944(char *p);

void func_08A8D27C(A6TgSelf2 *self, unsigned char on) {
    if (on == 1) {
        func_0887B924(self->field_08->field_254);
    } else {
        func_0887B944(self->field_08->field_254);
    }
}
