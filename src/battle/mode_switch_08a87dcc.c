typedef struct ModeCtrl {
    char pad_000[0x4];
    void *body;
    char pad_008[0x24];
    char lock[0x104];
    char effect[0xF0];
    void *current;
    char pad_224[0xC];
    char ground[0x320];
    char air[0x4];
} ModeCtrl;

extern int func_08A7E904(void *lock, int flags);
extern void func_08A7E858(void *lock, int flags);
extern int func_08A84124(void *body);
extern void func_08A8A6A0(void *mode);
extern void func_08A8F930(void *mode, int on);
extern void func_08A958C0(void *effect);

/* Unless locked or already in the air mode, switches to the ground or air mode, then releases the lock. */
void func_08A87DCC(ModeCtrl *self)
{
    int none = 0;
    if (func_08A7E904(self->lock, none) == 0 && self->current != self->air) {
        if (func_08A84124(self->body) == 0) {
            func_08A8A6A0(self->ground);
        } else {
            func_08A8F930(self->air, 1);
        }
        func_08A958C0(self->effect);
    }
    func_08A7E858(self->lock, none);
}
