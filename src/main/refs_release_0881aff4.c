/* Releases both held resources, then finishes the shutdown. */
typedef struct Holder0881AFF4 {
    char pad_00[8];
    void *second;
    void *first;
} Holder0881AFF4;

void func_08842138(void *resource);
void func_0881AFDC(Holder0881AFF4 *self);

void func_0881AFF4(Holder0881AFF4 *self)
{
    if (self->first != 0) {
        func_08842138(self->first);
    }
    if (self->second != 0) {
        func_08842138(self->second);
    }
    func_0881AFDC(self);
}
