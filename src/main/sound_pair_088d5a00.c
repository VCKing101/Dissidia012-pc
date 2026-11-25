/* Logs the two messages, then (when an entry index is given) plays the entry's two sounds, using the short or the long variant depending on the count. */
typedef struct P { char p0[0x1C]; int snd[1]; } P;
extern void func_088D3F00(int a, void *p, int b);
extern void func_088D9CD8(int s);
extern void func_088D9CFC(int s);
void func_088D5A00(int a, P *p, int b, int c, unsigned char flag, unsigned short idx, int n)
{
    P *e;
    func_088D3F00(a, p, b);
    if (flag != 0) {
        func_088D3F00(a, p, c);
    }
    if (idx != 0xFFFF) {
        e = (P *)((char *)p + idx * 4);
        if (n > 0 && n < 0x23) {
            func_088D9CD8(e->snd[0]);
            func_088D9CD8(e->snd[1]);
        } else {
            func_088D9CFC(e->snd[0]);
            func_088D9CFC(e->snd[1]);
        }
    }
}
