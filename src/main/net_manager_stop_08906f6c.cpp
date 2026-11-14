/* Closes the three connections and the session, then requests shutdown (phase 7). */
struct NetConnection {
    char data[0x2E0];
};

struct NetManager {
    int handle;
    char pad_0004[0x808 - 4];
    NetConnection connections[3];
    char pad_10A8[0x10D8 - 0x10A8];
    char session[0x1658 - 0x10D8];
    int phase;
};

extern "C" {
extern void func_089066F8(NetConnection *c);
extern void func_0890666C(NetConnection *c);
extern void func_0890C420(void *session);
extern void func_08907E24(void);
extern int func_0890314C(int handle, void (*done)(void), NetManager *m);

bool func_08906F6C(NetManager *m)
{
    for (int i = 0; i < 3; i++) {
        func_089066F8(&m->connections[i]);
        func_0890666C(&m->connections[i]);
    }
    func_0890C420(m->session);
    if (func_0890314C(m->handle, func_08907E24, m) == 1) {
        m->phase = 7;
        return true;
    }
    return false;
}
}
