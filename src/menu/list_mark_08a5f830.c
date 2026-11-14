typedef struct MarkList {
    char pad_000[0x3C];
    int field_3C[0x5D];
} MarkList;

extern void func_088D9F5C(int target, int kind, int value);
extern int func_08A5F8C0(MarkList *self);

void func_08A5F830(MarkList *self)
{
    int i;
    int row;
    for (i = 2; i < 0x5D; i++) {
        func_088D9F5C(self->field_3C[i], 6, 0);
    }
    row = func_08A5F8C0(self) + 2;
    if (row >= 2 && row < 0x5D) {
        func_088D9F5C(self->field_3C[row], 4, 0);
    }
}
