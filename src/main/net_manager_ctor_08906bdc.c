/* Constructor of the network manager. */
typedef struct NetManager {
    int field_0000;
    char peers[0x10 * 0x80];
    int field_0804;
    char connections[3 * 0x2E0];
    char timers[0x10D0 - 0x10A8];
    void *listener_vtable;
    struct NetManager *listener_owner;
    char session[0x1644 - 0x10D8];
    int field_1644;
    char pad_1648[0x1654 - 0x1648];
    int field_1654;
    int phase;
    int field_165C;
    int field_1660;
    unsigned char field_1664;
    char pad_1665[0x16D8 - 0x1665];
    int field_16D8;
    int field_16DC;
    int field_16E0;
    unsigned char field_16E4;
    unsigned char field_16E5;
} NetManager;

extern char D_089CDCC0[];
extern void func_08906BB4(void);
extern void func_08906594(void);
extern void func_08906BA0(void);
extern void func_08993560(void *array, int count, int size, void (*ctor)(void));
extern void func_0890C384(void *session);

NetManager *func_08906BDC(NetManager *m)
{
    m->field_0000 = 0;
    func_08993560(m->peers, 0x10, 0x80, func_08906BB4);
    m->field_0804 = 0;
    func_08993560(m->connections, 3, 0x2E0, func_08906594);
    func_08993560(m->timers, 3, 0xC, func_08906BA0);
    m->listener_vtable = D_089CDCC0;
    m->listener_owner = m;
    func_0890C384(m->session);
    m->field_1644 = -1;
    m->field_1654 = -1;
    m->phase = 0;
    m->field_165C = -1;
    m->field_1660 = 0;
    m->field_16D8 = 0;
    m->field_16DC = 0;
    m->field_16E0 = 0;
    m->field_16E4 = 0;
    m->field_16E5 = 0;
    m->field_1664 = 0;
    return m;
}
