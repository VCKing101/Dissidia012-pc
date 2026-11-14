typedef struct LinkState {
    unsigned char active;
    char pad_01[0x13];
    unsigned char flag_14;
    char pad_15[0x23];
    void *first;
} LinkState;

extern "C" {
extern void *func_08835778(void *unit);
extern int func_088BF4CC(void *status, int id);
}

struct StatusCheck {
    static bool has(void *unit, int id) { return func_088BF4CC(func_08835778(unit), id) != 0; }
};

extern "C" int func_08AC0620(LinkState *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (StatusCheck::has(self->first, 0xF0) == true) {
        unsigned char one = 1;
        self->flag_14 = one;
    }
    return 0;
}
