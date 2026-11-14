/* Starts the fighter's burst (state 2, flag 0x10) and queues the burst
 * command block with this value (-1 closed, -2 not allowed, -3 no fighter). */
typedef struct BurstInfo {
    unsigned char pad_00[4];
    struct BurstFighter *fighter;
} BurstInfo;

typedef struct BurstFighter {
    unsigned char pad_000[0x188];
    unsigned int flags;
} BurstFighter;

typedef struct BurstEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} BurstEvent;

extern int func_088B39C0(void);
extern BurstInfo *func_08AB3264(void *owner);
extern void func_08A65A84(void *queue);
extern void func_08B34E98(BurstFighter *fighter, int state);
extern void func_08A65AAC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65C08(void *queue, int value);
extern void func_08A65AFC(void *queue);

int func_08A6ADB4(BurstEvent *self, unsigned char value)
{
    BurstFighter *fighter;
    if (!self->open) {
        return -1;
    }
    if (func_088B39C0() == 0) {
        return -2;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -3;
    }
    func_08A65A84(self->queue);
    func_08B34E98(fighter, 2);
    fighter->flags |= 0x10;
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x4F);
    func_08A65C08(self->queue, value);
    func_08A65AFC(self->queue);
    return 0;
}
