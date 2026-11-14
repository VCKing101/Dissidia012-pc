typedef struct HandleOwner69D4 {
    char pad_000[0x8];
    void *field_08;
} HandleOwner69D4;

extern "C" void func_088DA060(void *handle);

struct HandleSlot {
    static void release(void **slot)
    {
        if (*slot) {
            func_088DA060(*slot);
            *slot = 0;
        }
    }
};

extern "C" {

int func_08A069D4(HandleOwner69D4 *self)
{
    if (self->field_08) {
        HandleSlot::release(&self->field_08);
    }
    return 0;
}

}
