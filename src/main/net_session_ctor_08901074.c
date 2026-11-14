/* Constructor: default addresses, the socket at 0x68, then the rest. */
typedef struct NetSession {
    char pad_00[0x28];
    unsigned long long addr[3];
    char pad_40[0x68 - 0x40];
    char socket[4];
} NetSession;

extern unsigned long long D_089CDB60;
extern void func_0891A810(void *socket);
extern void func_089010CC(NetSession *s);

NetSession *func_08901074(NetSession *s)
{
    s->addr[0] = D_089CDB60;
    s->addr[1] = D_089CDB60;
    s->addr[2] = D_089CDB60;
    func_0891A810(s->socket);
    func_089010CC(s);
    return s;
}
