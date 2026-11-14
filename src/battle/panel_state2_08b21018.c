typedef struct PanelState {
    int field_00;
    int state;      /* 0x04 */
} PanelState;

extern int func_08B21010(PanelState *slot);

int func_08B21018(PanelState *slot)
{
    if (func_08B21010(slot) == 0) {
        return 0;
    }
    return slot->state == 2;
}
