struct CostCheck {
    char pad_00[0x14];
    int id;
};

extern "C" {

extern int func_08AA4F88(CostCheck *self);
extern void *func_08AB3264(int id);
extern int func_08A976C4(void *slot);
extern int func_08AA4FF4(CostCheck *self, unsigned char kind);

/* True when the unit can pay the cost and kind has a slot. */
bool func_08AA4ED0(CostCheck *self, unsigned char kind)
{
    int cost = func_08AA4F88(self);
    if (cost <= 0) {
        return false;
    }
    if (func_08A976C4(func_08AB3264(self->id)) < cost) {
        return false;
    }
    return func_08AA4FF4(self, kind) >= 0;
}

}
