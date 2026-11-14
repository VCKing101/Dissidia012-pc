typedef struct SwapPart {
    char data[0x7C];
} SwapPart;

typedef struct SwapPair {
    char pad_000[0x8];
    SwapPart field_08[2];
    char pad_100[0x1DC - 0x100];
    int field_1DC;
} SwapPair;

extern void func_08A76078(SwapPart *part, int mode);

void func_08A7684C(SwapPair *self, int delta)
{
    int previous = self->field_1DC;
    self->field_1DC = (self->field_1DC + delta + 2) % 2;
    func_08A76078(&self->field_08[previous], 0);
    func_08A76078(&self->field_08[self->field_1DC], 1);
}
