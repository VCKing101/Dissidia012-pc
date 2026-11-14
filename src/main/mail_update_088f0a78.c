/* Per-frame mail update; returns the arrival flag. */
extern void *D_089D712C;
extern unsigned char D_089D7F1B;
extern void func_088AB9F8(void *save);
extern int func_088F07F4(void);
extern void func_088F08A8(int arg);
extern int func_088F1474(void);
extern void func_088F1E28(int arg);

int func_088F0A78(unsigned char notify)
{
    int arrived;

    func_088AB9F8(D_089D712C);
    arrived = func_088F07F4();
    if (arrived != 0) {
        func_088F08A8(0);
    }
    if (notify && D_089D7F1B != 0 && func_088F1474() == 0) {
        func_088F1E28(0);
        D_089D7F1B = 0;
    }
    return arrived;
}
