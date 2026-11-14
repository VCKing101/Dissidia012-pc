extern void func_08AC888C(void *object);

void func_08AC8828(void *object, unsigned int flags)
{
    if (object != 0 && (flags & 1) != 0) {
        func_08AC888C(object);
    }
}
