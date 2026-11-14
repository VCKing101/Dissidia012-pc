typedef struct StackCtx {
    char pad_000[0xB08];
    int field_B08;
} StackCtx;

typedef struct StackOwner {
    char pad_000[0x14C];
    StackCtx *field_14C;
} StackOwner;

extern void func_08ABC37C(StackOwner *self);
extern void func_08ABC828(StackOwner *self, int value);
extern int func_08ABC868(StackOwner *self);

void func_08ABC428(StackOwner *self)
{
    self->field_14C->field_B08 = 0;
    func_08ABC37C(self);
    func_08ABC828(self, 2);
    func_08ABC828(self, 1);
    func_08ABC868(self);
    func_08ABC868(self);
    func_08ABC868(self);
}
