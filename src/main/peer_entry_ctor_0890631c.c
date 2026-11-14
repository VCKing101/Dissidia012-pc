/* Constructor: vtable, the address record at 4 filled from `mac`. */
typedef struct MacBytes {
    signed char bytes[6];
} MacBytes;

typedef struct PeerEntry {
    void *vtable;
    MacBytes mac;
    char pad_0A[0x30 - 0xA];
    int state;
} PeerEntry;

extern char D_089CDC40[];
extern void func_08905884(MacBytes *mac);

PeerEntry *func_0890631C(PeerEntry *e, const MacBytes *mac)
{
    MacBytes *dst;

    e->vtable = D_089CDC40;
    dst = &e->mac;
    func_08905884(dst);
    e->state = 0;
    *dst = *mac;
    return e;
}
