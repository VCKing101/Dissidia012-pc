typedef struct SlotTable {
    char pad_00[0x4C];
    unsigned int count;
} SlotTable;

typedef struct SlotPick {
    unsigned char active;
    char pad_01[0x13];
    int mode;
    char pad_18[0x3C];
    unsigned int slot;
} SlotPick;

extern int D_08AAAD80;
extern SlotTable *func_08AA79C4(int key);
extern unsigned int func_08A81164(SlotPick *self, int kind, int id);

/* Picks the kind 0 slot for id and switches to mode 2; -1 inactive, -2 no free slot. */
int func_08A80D18(SlotPick *self, int id)
{
    unsigned int count;
    unsigned int slot;
    if (self->active == 0) {
        return -1;
    }
    count = func_08AA79C4(D_08AAAD80)->count;
    slot = func_08A81164(self, 0, id);
    if (slot >= count) {
        return -2;
    }
    self->mode = 2;
    self->slot = slot;
    return 0;
}
