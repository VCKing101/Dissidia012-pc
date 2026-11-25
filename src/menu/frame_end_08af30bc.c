/* Per-frame end hook: starts a timed call if none is pending, clears a pending flag and runs the registered callback. */
typedef struct G30BC { char pad0[0xCC]; void (*cb)(int); char pad1[0x9C0 - 0xD0]; int pending; char pad2[0xA5C - 0x9C4]; unsigned char flag; } G30BC;
extern G30BC D_09B6CDA0;
extern float D_089C9F54;
extern int D_089D6C88;
extern int D_089D7830;
extern void func_088A18B0(int, int, int);
extern void func_088D16A4(int);
void func_08AF30BC(void)
{
    if (D_09B6CDA0.pending == 0) {
        func_088A18B0(D_089D6C88, 0x64, (int)D_089C9F54);
    }
    if (D_09B6CDA0.flag) {
        func_088D16A4(D_089D7830);
        D_09B6CDA0.flag = 0;
    }
    D_09B6CDA0.cb(0);
}
