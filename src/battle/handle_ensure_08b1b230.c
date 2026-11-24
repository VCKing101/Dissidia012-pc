/* In state 2, ensures the handle at +0x20 (kind 0xB) exists or switches it from mode 9; -1 inactive, -2 wrong state, -3 open failed. */
typedef struct HSub { char pad_00[0xC]; short cur; short mode; } HSub;
typedef struct HSlot { unsigned char active; char pad[3]; int state; char pad_08[0x18]; HSub *handle; char pad_24[8]; int owner; } HSlot;
extern HSub *func_088DAA40(int owner, int kind, int z);
extern void func_088D9F5C(HSub *h, int a, int b);
int func_08B1B230(HSlot *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state != 2) {
        return -2;
    }
    if (self->handle == 0) {
        self->handle = func_088DAA40(self->owner, 0xB, 0);
        if (self->handle == 0) {
            return -3;
        }
    } else if (self->handle->cur == 9) {
        self->handle->mode = 0xB;
    } else {
        func_088D9F5C(self->handle, 0xB, 1);
    }
    return 0;
}
