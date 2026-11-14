typedef struct WaitContext {
    char pad_000[0x1B0];
    char field_1B0[4];
} WaitContext;

typedef struct WaitPart {
    char pad_000[0x30];
    char field_30[0x2C];
    int field_5C;
    char pad_60[0x8];
    int field_68;
} WaitPart;

extern int func_08A77370(char *anim);
extern void func_08A79E68(char *list, int kind);

void func_08A7E758(WaitPart *self, WaitContext *context)
{
    if (func_08A77370(self->field_30) == 1) {
        switch (self->field_5C) {
        case 0:
            func_08A79E68(context->field_1B0, 10);
            break;
        case 1:
            self->field_68 = 0;
            break;
        }
    }
}
