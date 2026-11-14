/* Ready check: -1 when not ready, -2 when the timer at 0x24 fails to restart. */
typedef struct SlotState {
    char pad_00[0x10];
    int state;
    char pad_14[0x24 - 0x14];
    char timer[0x28];
} SlotState;

extern int func_088D723C(SlotState *slot);
extern int func_088D8460(void *timer);

int func_088D7574(SlotState *slot)
{
    if (func_088D723C(slot) == 0) {
        return -1;
    }
    if (slot->state != 6 && func_088D8460(slot->timer) != 0) {
        return -2;
    }
    return 0;
}
