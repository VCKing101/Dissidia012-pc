typedef struct LinkState {
    unsigned char active;
    char pad_01[0x37];
    void *first;
} LinkState;

extern "C" {
extern void *func_08835778(void *unit);
extern int func_088BF4CC(void *status, int id);
}

struct StatusCheck {
    static bool has(void *unit) { return func_088BF4CC(func_08835778(unit), 0xEC) != 0; }
};

/* 1 unless the first unit has status 0xEC. */
extern "C" int func_08AC07EC(LinkState *self)
{
    return !StatusCheck::has(self->first);
}
