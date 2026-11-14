typedef struct VoiceSlot {
    int field_00;
    int state;      /* 0x04 */
} VoiceSlot;

extern int func_08B1DBC8(VoiceSlot *slot);

int func_08B1DBD0(VoiceSlot *slot)
{
    if (func_08B1DBC8(slot) == 0) {
        return 0;
    }
    return slot->state == 5;
}
