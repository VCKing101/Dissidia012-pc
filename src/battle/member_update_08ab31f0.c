typedef struct UpdateOwner {
    unsigned char active;
    char pad_001[0x3];
    char timer_004[0x30];
    int mode;
    char pad_038[0x160];
    char slots_198[0x1D4];
    char track_36C[0x24];
    char guard_390[0xB0];
    char links_440[1];
} UpdateOwner;

extern void func_08A972D4(void *timer);
extern void func_08A82558(void *slots);
extern void func_08A7EB98(void *guard);
extern void func_08A86EA4(void *links);
extern void func_08A7B33C(void *track);

/* Per-frame update of the owner's members; the guard only in mode 2. */
int func_08AB31F0(UpdateOwner *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A972D4(self->timer_004);
    func_08A82558(self->slots_198);
    if (self->mode == 2) {
        void *guard = self->guard_390;
        func_08A7EB98(guard);
    }
    func_08A86EA4(self->links_440);
    func_08A7B33C(self->track_36C);
    return 0;
}
