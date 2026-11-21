/* Sets the status panel's state, playing the header animation for the
 * new state (open for 0, flash for 3 and 4); state 4 is final. */
typedef struct StateLayout {
    unsigned char pad_00[0xE];
    short frame;
} StateLayout;

typedef struct StateParts {
    unsigned char pad_00[0x18];
    int state;
    unsigned char pad_1c[0x38];
    StateLayout *header;
} StateParts;

typedef struct StateEffect {
    unsigned char pad_00[0x14];
    StateParts *parts;
} StateEffect;

extern void func_088D9F5C(void *sprite, int frame, int flags);

void func_08A43BD8(StateEffect *self, int state)
{
    if (self->parts->state == 4) {
        return;
    }
    switch (state) {
    case 0:
        func_088D9F5C(self->parts->header, 0x16, 1);
        self->parts->header->frame = 0x17;
        break;
    case 3:
        func_088D9F5C(self->parts->header, 0x18, 1);
        break;
    case 4:
        if (state != 3) {
            func_088D9F5C(self->parts->header, 0x18, 1);
        }
        break;
    }
    self->parts->state = state;
}
