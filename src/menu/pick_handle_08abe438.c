typedef struct PickCtx {
    char pad_000[0x400];
    int field_400[1];
} PickCtx;

typedef struct PickOwner {
    char pad_000[0x14C];
    PickCtx *field_14C;
} PickOwner;

extern void func_088D9CD8(int handle);
extern void func_08ABE47C(PickOwner *self);

void func_08ABE438(PickOwner *self, int index)
{
    func_08ABE47C(self);
    func_088D9CD8(self->field_14C->field_400[index]);
}
