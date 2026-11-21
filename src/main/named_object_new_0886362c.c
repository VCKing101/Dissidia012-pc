extern char D_089C9298[];
extern char D_089C929C[];
extern char D_089C92A0[];
extern int func_08935970(const char *a, const char *b);   /* strcmp */
extern void *func_08864C00(int size);                     /* operator new */
extern void func_088631D0(void *object);                  /* constructor */

void *func_0886362C(const char *name)
{
    if (func_08935970(name, D_089C9298) != 0) {
        if (func_08935970(name, D_089C929C) == 0 || func_08935970(name, D_089C92A0) == 0) {
            void *result = 0;
            void *object = func_08864C00(0x160);

            if (object != 0) {
                func_088631D0(object);
                result = object;
            }
            return result;
        }
    }
    return 0;
}
