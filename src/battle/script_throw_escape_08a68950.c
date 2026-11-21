/* Queues the fighter's throw-escape script (0x2B, then 0x2D/0x2C, then 1, 7)
 * when it is held or thrown (-1 closed, -2 no fighter, -3 neither). */
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
extern int func_08B2F5E0(void *fighter);
extern int func_08B2F7E0(void *fighter);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A68950(CrushEvent *self)
{
    void *fighter;
    if (!self->open) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (func_08B2F5E0(fighter) == 0 && func_08B2F7E0(fighter) == 0) {
        return -3;
    }
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x2B);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x2D);
    func_08A65B50(self->queue, 0x2C);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 1);
    func_08A65C08(self->queue, 7);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    return 0;
}
