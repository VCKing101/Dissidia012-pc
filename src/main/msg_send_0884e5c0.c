/* Builds a message in the shared buffer and posts it unless posting is muted. */
extern unsigned char D_089D4D9C;
extern int D_089D4DA0;
extern char D_09B12B88[];

void func_08935524(void *buf, int a, int b);
void func_0884E558(int queue, void *buf);

void func_0884E5C0(int a, int b)
{
    if (D_089D4D9C != 0) {
        return;
    }
    func_08935524(D_09B12B88, a, b);
    func_0884E558(D_089D4DA0, D_09B12B88);
}
