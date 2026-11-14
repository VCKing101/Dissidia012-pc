/* Allocation and release wrappers for the battle setup root. */
extern void *func_08890900(unsigned int);
extern void func_08890970(void *);

void *func_089DD67C(unsigned int size)
{
    return func_08890900(size);
}

void func_089DD698(void *object)
{
    func_08890970(object);
}
