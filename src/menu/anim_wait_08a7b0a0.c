typedef struct WaitContext {
    char pad_000[0x1B0];
    char field_1B0[4];
} WaitContext;

typedef struct WaitPart {
    char pad_000[0x80];
    char field_80[0x2C];
    int field_AC;
    char pad_B0[0x8];
    int field_B8;
} WaitPart;

extern int func_08A77370(char *anim);
extern void func_08A79E68(char *list, int kind);

void func_08A7B0A0(WaitPart *self, WaitContext *context)
{
    if (func_08A77370(self->field_80) == 1) {
        switch (self->field_AC) {
        case 0:
            func_08A79E68(context->field_1B0, 10);
            break;
        case 1:
            self->field_B8 = 1;
            break;
        }
    }
}
