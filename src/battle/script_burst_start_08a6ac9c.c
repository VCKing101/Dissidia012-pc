/* Starts the fighter's burst in the given order slot: burst state and flag,
 * then the burst script blocks (0x4B; 0x1C, 3 at the burst rate; 0x1A,
 * 0x1D) (-1 closed, -2 no fighter). */
typedef struct BurstFighter {
    unsigned char pad_000[0x188];
    unsigned int flags;
} BurstFighter;

typedef struct CrushInfo {
    unsigned char pad_00[4];
    BurstFighter *fighter;
    unsigned char pad_08[0x18];
    int order;
} CrushInfo;

typedef struct CrushEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} CrushEvent;

extern CrushInfo *func_08AB3264(void *owner);
extern void func_08B34E98(BurstFighter *fighter, int state);
extern int func_08A65C64(void *queue, float rate);
extern float D_08A7ABEC;
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A6AC9C(CrushEvent *self, int order)
{
    BurstFighter *fighter;
    if (!self->open) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    func_08AB3264(self->owner)->order = order;
    func_08A65A84(self->queue);
    func_08B34E98(fighter, 2);
    fighter->flags |= 0x10;
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x4B);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x1C);
    func_08A65B50(self->queue, 3);
    func_08A65C64(self->queue, D_08A7ABEC);
    func_08A65BAC(self->queue, 0xD);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x1A);
    func_08A65B50(self->queue, 0x1D);
    func_08A65AFC(self->queue);
    return 0;
}
