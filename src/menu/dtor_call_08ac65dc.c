extern void func_08992F04(void *object);

void func_08AC65DC(void *object, unsigned int flags)
{
    if (object != 0 && (flags & 1) != 0) {
        func_08992F04(object);
    }
}
