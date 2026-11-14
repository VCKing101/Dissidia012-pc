typedef struct WaitContext {
    char pad_000[0x1B0];
    char field_1B0[4];
} WaitContext;

typedef struct WaitPart {
    char pad_000[0x1E4];
    char field_1E4[0x2C];
    int field_210;
    char pad_214[0x14];
    int field_228;
} WaitPart;

extern int func_08A77370(char *anim);
extern void func_08A79E68(char *list, int kind);

void func_08A7D0D8(WaitPart *self, WaitContext *context)
{
    if (func_08A77370(self->field_1E4) == 1) {
        switch (self->field_210) {
        case 0:
            func_08A79E68(context->field_1B0, 10);
            break;
        case 1:
            self->field_228 = 1;
            break;
        }
    }
}
