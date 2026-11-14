/* Looks up the table value for (row at 0x14, the timer's limit as column). */
typedef struct SlotLookup {
    char pad_00[0xC];
    void *timer;
    int field_10;
    int row;
} SlotLookup;

extern int D_089CC2F8[][2];
extern unsigned int func_088DF2F8(void *timer);

int func_088DEFF0(SlotLookup *slot)
{
    unsigned int column = func_088DF2F8(slot->timer);

    if (column >= 2) {
        return -1;
    }
    return D_089CC2F8[slot->row][column];
}
