/* Reads a system time struct and stores one of its words in a global. */
extern void func_089C5D98(void *p);
extern int D_09B6F454;
void func_08AB09E0(void) {
    int buf[4];
    func_089C5D98(buf);
    D_09B6F454 = buf[3];
}
