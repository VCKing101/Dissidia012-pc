/* Flags every voice of the group (or of all groups for 0xFF) in both lists with bit 1. */
typedef struct Voice088CF750 {
    char pad_00[0x14];
    unsigned char flags;
    char pad_15;
    unsigned char group;
    char pad_17[0x24 - 0x17];
    struct Voice088CF750 *next;
} Voice088CF750;

typedef struct Mixer088CF750 {
    char pad_00[0x24];
    Voice088CF750 *active;
    char pad_28[0x30 - 0x28];
    Voice088CF750 *waiting;
} Mixer088CF750;

void func_088CF750(Mixer088CF750 *m, unsigned char group)
{
    Voice088CF750 *v;
    Voice088CF750 *next;
    for (v = m->active; v != 0; v = next) {
        next = v->next;
        if (v->group == group || group == 0xFF) {
            v->flags |= 2;
        }
    }
    for (v = m->waiting; v != 0; v = next) {
        next = v->next;
        if (v->group == group || group == 0xFF) {
            v->flags |= 2;
        }
    }
}
