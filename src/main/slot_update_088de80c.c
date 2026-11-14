/* Per-frame update: finish when the timer at 0x14 expires, else advance to state 2. */
typedef struct SlotUpdateB {
    char pad_00[0xC];
    int state;
    char pad_10[0x14 - 0x10];
    char timer[0x80 - 0x14];
    char watch[0x10];
} SlotUpdateB;

extern int func_088DF2C0(void *timer);
extern void func_088DE8F4(SlotUpdateB *slot);
extern int func_088DF860(void *watch);
extern int func_088DF3B8(void *timer);

int func_088DE80C(SlotUpdateB *slot)
{
    void *timer = slot->timer;

    if (func_088DF2C0(timer) != 0) {
        func_088DE8F4(slot);
    } else if (func_088DF860(slot->watch) == 0) {
        func_088DF3B8(timer);
        slot->state = 2;
    }
    return 0;
}
