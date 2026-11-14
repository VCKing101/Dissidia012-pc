/* Registers an effect type with its factory and type info. */
extern char D_089CB228[];

void func_088A983C(void);
void func_088823C4(int a, int b, int c, void (*factory)(void), void *type);

void func_088A9814(int a, int b, int c)
{
    func_088823C4(a, b, c, func_088A983C, D_089CB228);
}
