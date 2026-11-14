/* Queries the model's effect into a 16-entry buffer with the unit's filter. */
struct Effect088386C8 {
    char pad_00[0x28];
    void *handle;
};

struct Model088386C8 {
    char pad_00[0x34];
    Effect088386C8 effect;

    static Effect088386C8 *effectOf(Model088386C8 *m) { return &m->effect; }
};

struct Unit088386C8 {
    char pad_000[0x390];
    Model088386C8 *model;
    char pad_394[0x134];
    char filter[4];
};

extern "C" {

void func_0883957C(void);
void func_088A9E60(void *handle, void *buf, int count, void (*cb)(void), int a, void *filter, int b, int c);

void func_088386C8(Unit088386C8 *u)
{
    int buf[16];
    void *handle = Model088386C8::effectOf(u->model)->handle;
    if (handle != 0) {
        func_088A9E60(handle, buf, 0x10, func_0883957C, 0, u->filter, 0, 0);
    }
}

}
