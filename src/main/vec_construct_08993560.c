extern void *D_089DA450;

/* The C++ runtime's general array constructor (cookie, destructor and allocation hooks). */
extern void *func_08993300(void *array, int count, int size, int a3, void (*ctor)(void *), int t1, int t2,
                           int t3, int s0, int s1, int s2, void *hook);

/* vec_construct: construct `count` objects of `size` bytes at `array` with `ctor` (what
 * SNC emits for a member array of a class with a constructor). */
void *func_08993560(void *array, int count, int size, void (*ctor)(void *))
{
    return func_08993300(array, count, size, 0, ctor, 0, 0, 0, 0, 0, 0, D_089DA450);
}
