typedef struct SlotWatch {
    unsigned char active;
} SlotWatch;

extern void func_08AA211C(SlotWatch *self);
extern void func_08AA23F8(SlotWatch *self);

int func_08AA20B8(SlotWatch *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08AA211C(self);
    func_08AA23F8(self);
    return 0;
}
