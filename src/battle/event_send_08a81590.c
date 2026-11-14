typedef struct EventSend81590 {
    char pad_000[0x8];
    int field_08;
} EventSend81590;

extern int func_08A969BC(int id, int value);

int func_08A81590(EventSend81590 *self)
{
    if (func_08A969BC(self->field_08, 0)) {
        return -1;
    }
    return 0;
}
