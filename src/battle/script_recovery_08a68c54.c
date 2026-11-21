/* Queues the fighter's recovery script (0x32, a longer wait in mode 1, the
 * motion block 0xF, 7, 0x33, and in mode 0 the end block 1, 0xB) (-1
 * closed, -2 no fighter, -3 the fighter cannot recover). */
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
extern int func_08AB3540(void *fighter);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A68C54(CrushEvent *self, unsigned char mode)
{
    void *fighter;
    if (!self->open) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (func_08AB3540(fighter) == 0) {
        return -3;
    }
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x32);
    func_08A65BAC(self->queue, 0xC);
    if (mode == 1) {
        func_08A65BAC(self->queue, 0x14);
    }
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0xF);
    func_08A65C08(self->queue, 7);
    func_08A65B50(self->queue, 0x33);
    func_08A65BAC(self->queue, 0xC);
    func_08A65AFC(self->queue);
    if (mode == 0) {
        func_08A65AAC(self->queue);
        func_08A65B50(self->queue, 1);
        func_08A65C08(self->queue, 0xB);
        func_08A65BAC(self->queue, 0xC);
        func_08A65AFC(self->queue);
    }
    return 0;
}
