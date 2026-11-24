/* Ensures the slot's handle (kind 6) exists or refreshes it; -1 inactive, -2 open failed. */
typedef struct HSub { char pad_00[0xE]; short mode; } HSub;
typedef struct HSlot { unsigned char active; char pad[3]; int owner; HSub *handle; } HSlot;
extern HSub *func_088DAA40(int owner, int kind, int z);
extern void func_088D9F5C(HSub *h, int a, int b);
int func_08B1C21C(HSlot *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->handle == 0) {
        self->handle = func_088DAA40(self->owner, 6, 0);
        if (self->handle == 0) {
            return -2;
        }
    } else {
        func_088D9F5C(self->handle, 6, 1);
    }
    return 0;
}
