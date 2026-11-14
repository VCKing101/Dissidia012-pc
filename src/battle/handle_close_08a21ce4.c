typedef struct HandleClose21CE4 {
    char pad_000[0x8];
    void *field_08;
} HandleClose21CE4;

extern void func_08890970(void *handle);
extern void func_08A21CD0(HandleClose21CE4 *self);

void func_08A21CE4(HandleClose21CE4 *self)
{
    if (self->field_08) {
        func_08890970(self->field_08);
    }
    func_08A21CD0(self);
}
