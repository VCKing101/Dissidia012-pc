typedef struct SlotEntry22574 {
    char data[0x28];
} SlotEntry22574;

typedef struct SlotOwner22574 {
    unsigned char active;
    char pad_001[0x487];
    SlotEntry22574 *field_488;
} SlotOwner22574;

extern int func_08891284(SlotEntry22574 *entry);
extern int func_088912D0(SlotEntry22574 *entry);

int func_08A22574(SlotOwner22574 *self, unsigned int index)
{
    if (!self->active) {
        return 0;
    }
    if (index >= 2) {
        return 0;
    }
    return func_08891284(&self->field_488[index]);
}

int func_08A225D0(SlotOwner22574 *self, unsigned int index)
{
    if (!self->active) {
        return 0;
    }
    if (index >= 2) {
        return 0;
    }
    return func_088912D0(&self->field_488[index]);
}
