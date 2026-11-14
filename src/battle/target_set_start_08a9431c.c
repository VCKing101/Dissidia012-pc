typedef struct TargetSet {
    unsigned char active;
} TargetSet;

extern void func_08A94300(TargetSet *self);
extern int func_08A945D4(TargetSet *self, int mode);

int func_08A9431C(TargetSet *self)
{
    func_08A94300(self);
    self->active = 1;
    if (func_08A945D4(self, 1) != 0) {
        return -4;
    }
    return 0;
}
