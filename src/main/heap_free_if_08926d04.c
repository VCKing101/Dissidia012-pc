typedef struct HeapOwner { int field_00; void *heap; } HeapOwner;
extern void func_0885C538(void *heap, void *data, int flags);
void func_08926D04(HeapOwner *owner, void *data)
{
    void *p = data;
    if (p != 0) {
        func_0885C538(owner->heap, p, 0);
    }
}
