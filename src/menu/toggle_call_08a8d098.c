typedef struct A6TgInner {
    char pad_000[0x254];
    char *field_254;
    char *field_258;
    char *field_25C;
} A6TgInner;

typedef struct A6TgSelf {
    char pad_00[8];
    A6TgInner *field_08;
} A6TgSelf;

void func_0887B924(char *p);
void func_0887B944(char *p);

void func_08A8D098(A6TgSelf *self, unsigned char on) {
    if (on == 1) {
        func_0887B924(self->field_08->field_25C);
    } else {
        func_0887B944(self->field_08->field_25C);
    }
}
