typedef struct LinkState {
    unsigned char active;
    char pad_01[0x37];
    void *first;
    void *second;
    void *third;
} LinkState;

extern void func_08AC045C(LinkState *self);

/* Reset, then bind all three objects; -1 when any is missing. */
int func_08AC0478(LinkState *self, void *first, void *second, void *third)
{
    func_08AC045C(self);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    self->first = first;
    self->second = second;
    self->third = third;
    self->active = 1;
    return 0;
}
