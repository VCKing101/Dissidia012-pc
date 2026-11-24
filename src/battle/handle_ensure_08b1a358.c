/* Ensures the slot's handle exists (opening it if absent) or updates it, then refreshes; -1 inactive, -2 open failed. */
typedef struct HSub { char pad_00[0xE]; short mode; } HSub;
typedef struct HSlot { unsigned char active; char pad[3]; int owner; HSub *handle; } HSlot;
extern HSub *func_088DAA40(int owner, int kind, int z);
extern int func_08B1A240(HSlot *s);
extern void func_088D9F5C(HSub *h, int a, int b);
extern void func_08B1A178(HSlot *s);
int func_08B1A358(HSlot *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->handle == 0) {
        self->handle = func_088DAA40(self->owner, 2, 0);
        if (self->handle == 0) {
            return -2;
        }
    } else {
        if (func_08B1A240(self) == 1) {
            self->handle->mode = 2;
        } else {
            func_088D9F5C(self->handle, 2, 1);
        }
    }
    func_08B1A178(self);
    return 0;
}
