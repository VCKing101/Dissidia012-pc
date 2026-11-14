/* Frees the effect array, the effect bank and the effect count. */
extern "C" {
void func_088A68B8(void *voice, unsigned int flags);
void func_0899372C(void *array, int count, int size, void (*dtor)(void *, unsigned int), int a, int b);
void func_0888218C(void *bank, int size);
extern void *D_09B4D434;
extern void *D_09B4D438;
extern int D_089D6D94;
}

struct Effects088A7FB8 {
    static void destroy(void *array)
    {
        if (array != 0) {
            func_0899372C(array, 0x40, 0x280, func_088A68B8, 0, 0);
        }
    }
};

extern "C" {

void func_088A7FB8(void)
{
    if (D_09B4D434 != 0) {
        Effects088A7FB8::destroy(D_09B4D434);
        D_09B4D434 = 0;
    }
    if (D_09B4D438 != 0) {
        func_0888218C(D_09B4D438, 0x80);
        D_09B4D438 = 0;
    }
    D_089D6D94 = 0;
}

}
