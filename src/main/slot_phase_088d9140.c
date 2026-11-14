/* Returns 9 when the timer's limit equals the stored target, else 8. */
typedef struct SlotPhase {
    char pad_00[0x30];
    void *timer;
    int field_34;
    int target;
} SlotPhase;

extern int func_088D82F0(void *timer);

int func_088D9140(SlotPhase *slot)
{
    if (func_088D82F0(slot->timer) == slot->target) {
        return 9;
    }
    return 8;
}
