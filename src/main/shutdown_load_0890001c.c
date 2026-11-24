/* Close the global load session: run the final step when the flag allows, then clear and notify. */
extern int D_089D8128;
extern unsigned char *D_089D7280;
extern int func_088FF1AC(unsigned char v);
extern void func_089A62C0(int p);
extern void func_088FFA60(void);
extern void func_08882EC8(void);
int func_0890001C(unsigned char v)
{
    int r = 0;
    if (D_089D8128 != 0) {
        if (D_089D7280[3] == 0) {
            r = func_088FF1AC(v);
            func_089A62C0(D_089D8128 + 0x83E80);
        }
        func_088FFA60();
        D_089D8128 = 0;
        func_08882EC8();
    }
    return r;
}
