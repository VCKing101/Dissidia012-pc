/* Release the pending object held by the global manager and re-enable its lock. */
typedef struct Held { char pad[0x40]; char flag; } Held;
typedef struct Mgr { char pad[0x8FC]; Held *held; void *lock; } Mgr;
extern Mgr *D_089DA420;
extern void func_089C5FD8(void *lock, int a, int b);
extern void func_0899286C(Held *h, int arg);
extern void func_089C5FC0(void *lock, int a);
void func_08990680(int arg)
{
    Mgr *m;
    func_089C5FD8(D_089DA420->lock, 1, 0);
    m = D_089DA420;
    if (m->held != 0) {
        char zero;
        func_0899286C(m->held, arg);
        m = D_089DA420;
        zero = 0;
        m->held->flag = zero;
        m->held = 0;
    }
    func_089C5FC0(m->lock, 1);
}
