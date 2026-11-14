typedef struct BlendRef {
    int field_00;
    void *child;    /* 0x04 */
} BlendRef;

extern void func_08B2CF9C(void *child);

void func_08B2CF5C(BlendRef *ref)
{
    if (ref == 0) {
        return;
    }
    if (ref->child != 0) {
        func_08B2CF9C(ref->child);
    }
}
