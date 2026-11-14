extern void func_089110FC(void *handle);

typedef struct HandleRef {
    int field_00;
    void *handle;
} HandleRef;

void func_0890F370(HandleRef *ref)
{
    if (ref->handle == 0) {
        return;
    }
    func_089110FC(ref->handle);
    ref->handle = 0;
    ref->field_00 = 0;
}
