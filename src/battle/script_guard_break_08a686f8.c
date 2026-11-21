/* Queues the fighter's guard-break script: the break block (0x29, 0x38, 2),
 * the crush motion block (0x2A, 0xF, 7) and the recovery block (0x38, 5, 0x31) (-1 closed, -2 no
 * fighter, -3 the fighter cannot act). */
typedef struct CrushInfo {
    unsigned char pad_00[4];
    void *fighter;
} CrushInfo;

typedef struct CrushEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} CrushEvent;

extern CrushInfo *func_08AB3264(void *owner);
extern int func_08AB344C(void *fighter);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A686F8(CrushEvent *self)
{
    void *fighter;
    if (!self->open) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (func_08AB344C(fighter) == 0) {
        return -3;
    }
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x29);
    func_08A65B50(self->queue, 0x38);
    func_08A65C08(self->queue, 2);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x2A);
    func_08A65B50(self->queue, 0xF);
    func_08A65C08(self->queue, 7);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x38);
    func_08A65C08(self->queue, 5);
    func_08A65BAC(self->queue, 0xC);
    func_08A65B50(self->queue, 0x31);
    func_08A65AFC(self->queue);
    return 0;
}
