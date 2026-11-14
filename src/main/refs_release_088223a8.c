/* Releases both held resources, then finishes the shutdown. */
typedef struct Holder088223A8 {
    char pad_00[0x28];
    void *second;
    void *first;
} Holder088223A8;

void func_08842138(void *resource);
void func_08822324(Holder088223A8 *self);

void func_088223A8(Holder088223A8 *self)
{
    if (self->first != 0) {
        func_08842138(self->first);
    }
    if (self->second != 0) {
        func_08842138(self->second);
    }
    func_08822324(self);
}
