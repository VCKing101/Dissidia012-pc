struct StatusBody {
    char pad_000[0x388];
    unsigned long long status;
};

struct StatusWatch {
    unsigned char active;
    static unsigned char hasStatus(StatusBody *body, unsigned long long mask) { return (body->status & mask) != 0; }
};

extern "C" {

extern unsigned long long D_08A98070;
extern StatusBody *func_08A973A8(StatusWatch *self);
extern void func_08A9748C(StatusWatch *self);
extern void func_08A974B0(StatusWatch *self);
extern void func_08A9752C(StatusWatch *self);

/* Resets the watcher's three parts while the body has one of the watched statuses; -1 inactive, -2 no body. */
int func_08A972D4(StatusWatch *self)
{
    if (self->active == 0) {
        return -1;
    }
    StatusBody *body = func_08A973A8(self);
    if (body == 0) {
        return -2;
    }
    if (StatusWatch::hasStatus(body, D_08A98070) == 1) {
        func_08A9748C(self);
        func_08A974B0(self);
        func_08A9752C(self);
    }
    return 0;
}

}
