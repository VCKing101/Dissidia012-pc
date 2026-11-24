// If the pointer stored at *slot is non-null, calls func_088DA060 on it and clears the slot.
extern void func_088DA060(void *);
void func_08B4166C(void **slot)
{
    if (*slot != 0) {
        func_088DA060(*slot);
        *slot = 0;
    }
}
