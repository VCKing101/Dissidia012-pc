/* Frees the voice array, the bank and the voice count. */
extern "C" {
void func_088A43EC(void *voice, unsigned int flags);
void func_0899372C(void *array, int count, int size, void (*dtor)(void *, unsigned int), int a, int b);
void func_08882158(void *bank);
extern void *D_089D6D58;
extern void *D_089D6D5C;
extern int D_089D6D60;
}

struct Voices088A4D04 {
    static void destroy(void *array)
    {
        if (array != 0) {
            func_0899372C(array, 0x40, 0x50, func_088A43EC, 0, 0);
        }
    }
};

extern "C" {

void func_088A4D04(void)
{
    if (D_089D6D58 != 0) {
        Voices088A4D04::destroy(D_089D6D58);
        D_089D6D58 = 0;
    }
    if (D_089D6D5C != 0) {
        func_08882158(D_089D6D5C);
        D_089D6D5C = 0;
    }
    D_089D6D60 = 0;
}

}
