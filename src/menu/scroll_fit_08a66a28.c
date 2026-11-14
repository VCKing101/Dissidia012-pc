/* Scrolls the list so the cursor sits two rows from the top, clamped, and redraws it. */
typedef struct FitList {
    char pad_00[0x50];
    int field_50;
    char pad_54[0x4];
    int field_58;
    char pad_5C[0x7C - 0x5C];
    int field_7C;
    char pad_80[0x4];
    int field_84;
} FitList;

extern void func_08A67540(FitList *self);
extern void func_08A67588(FitList *self);
extern void func_08A67420(FitList *self);

void func_08A66A28(FitList *self)
{
    int top = self->field_50 - 2;
    int rest = self->field_58 - self->field_50;
    if (rest < 6) {
        top = rest + top - 5;
    }
    if (top < 0) {
        top = 0;
    }
    self->field_7C = top;
    if (self->field_84 < top) {
        self->field_7C = self->field_84;
    }
    func_08A67540(self);
    func_08A67588(self);
    func_08A67420(self);
}
