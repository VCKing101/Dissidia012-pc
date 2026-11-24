/* Recomputes two cached values for each of the two slots from their stored ids, then finishes the update. */
struct Slot {
    char pad[0xF3A]; short cacheA; char pad1[2]; short cacheB; char pad2[0x118]; char a; char b; char c; char d; char pad3[4];
};
extern Slot D_09AFB8A0[];
extern "C" void func_088FB3C0(void);
extern "C" short func_088FB934(int a, int b, int kind);
extern "C" void func_088FB434(int a);
extern "C" void func_088F98E0(void)
{
    func_088FB3C0();
    for (int i = 0; i < 2; i++) {
        Slot *q = &D_09AFB8A0[i];
        q->cacheA = func_088FB934(q->a, q->b, 0);
        if (q->c >= 0) {
            q->cacheB = func_088FB934(q->c, q->d, 1);
        }
    }
    func_088FB434(0);
}
