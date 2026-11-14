typedef struct Holder { int field_00; void *data; } Holder;
extern void func_08925AF8(Holder *holder);
void func_0892507C(Holder *holder)
{
    if (holder->data == 0) {
        return;
    }
    func_08925AF8(holder);
}
