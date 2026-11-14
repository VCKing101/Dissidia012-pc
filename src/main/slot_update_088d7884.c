/* Per-frame update: finish when the timer at 0x24 expires, else advance to state 4. */
typedef struct SlotUpdate {
    char pad_00[0x10];
    int state;
    char pad_14[0x24 - 0x14];
    char timer[0x4C - 0x24];
    char watch[0x10];
} SlotUpdate;

extern int func_088D82B8(void *timer);
extern void func_088D7B00(SlotUpdate *slot);
extern int func_088D873C(void *watch);
extern int func_088D83D8(void *timer);

int func_088D7884(SlotUpdate *slot)
{
    void *timer = slot->timer;

    if (func_088D82B8(timer) != 0) {
        func_088D7B00(slot);
    } else if (func_088D873C(slot->watch) == 0) {
        func_088D83D8(timer);
        slot->state = 4;
    }
    return 0;
}
