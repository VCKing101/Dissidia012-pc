typedef struct StageState {
    int field_00;
    int state;      /* 0x04 */
} StageState;

extern int func_08B20118(StageState *slot);

int func_08B20120(StageState *slot)
{
    if (func_08B20118(slot) == 0) {
        return 0;
    }
    return slot->state == 3;
}
