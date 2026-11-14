/* Releases both held resources, then finishes the shutdown. */
typedef struct Holder08817D18 {
    char pad_00[0x20];
    void *second;
    void *first;
} Holder08817D18;

void func_08842138(void *resource);
void func_08817CD4(Holder08817D18 *self);

void func_08817D18(Holder08817D18 *self)
{
    if (self->first != 0) {
        func_08842138(self->first);
    }
    if (self->second != 0) {
        func_08842138(self->second);
    }
    func_08817CD4(self);
}
