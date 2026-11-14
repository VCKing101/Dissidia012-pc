extern void *func_088D0700(int heap, int size, int a, int b);
extern int D_089D7830;
void func_089114D0(void **out)
{
    int size = 0x24;
    *out = func_088D0700(D_089D7830, size, 0x8A1C, 0x8825);
    if (*out == 0) {
        return;
    }
}
