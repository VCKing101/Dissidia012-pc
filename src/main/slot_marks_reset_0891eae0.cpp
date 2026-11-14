/* Resets a selection record: five id/flag pairs, then two rows of three ids.
   The rows are addressed from the record base (base + index first), which the
   byte-offset form reproduces; `set->first[i]` adds the index first. */
extern "C" {

typedef struct MarkSlot {
    signed char id;
    unsigned char flag;
} MarkSlot;

typedef struct MarkSet {
    MarkSlot slots[5];          /* 0x00 */
    unsigned char field_0A;
    signed char first[3];       /* 0x0B */
    unsigned char field_0E;
    signed char second[3];      /* 0x0F */
    unsigned char field_12;
} MarkSet;

void func_0891EAE0(MarkSet *set)
{
    for (int i = 0; i < 5; i++) {
        MarkSlot *slot = &set->slots[i];
        slot->id = -1;
        slot->flag = 0;
    }
    set->field_0A = 0;
    for (int i = 0; i < 3; i++) {
        *((signed char *)set + 0xB + i) = -1;   /* first[i] */
        *((signed char *)set + 0xF + i) = -1;   /* second[i] */
    }
    set->field_0E = 0;
    set->field_12 = 0;
}

}
