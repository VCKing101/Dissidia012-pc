typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
} UnitHolder;

extern void func_08A97240(UnitHolder *self);
extern void func_08A9752C(UnitHolder *self);

int func_08A9727C(UnitHolder *self, void *owner)
{
    func_08A97240(self);
    if (owner == 0) {
        return -1;
    }
    self->owner = owner;
    self->active = 1;
    func_08A9752C(self);
    return 0;
}
