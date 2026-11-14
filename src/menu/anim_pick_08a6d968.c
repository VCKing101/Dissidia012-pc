typedef struct PickTarget {
    char pad_000[0x1D4];
    char field_1D4[4];
} PickTarget;

extern void func_08A6E2CC(char *anim, int kind, int a, int b);

void func_08A6D968(void *self, PickTarget *target, int state)
{
    switch (state) {
    case 7:
        func_08A6E2CC(target->field_1D4, 7, 0, 0);
        break;
    case 9:
        func_08A6E2CC(target->field_1D4, 3, 0, 0);
        break;
    }
}
