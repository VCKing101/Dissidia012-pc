/* Queues the slow-down script at the current motion's rate: the slow block
 * (9, 0xA) and the resume block (0xB, 0x1E, 1, 0x1A) (-1 closed). */
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
extern void *func_08AB327C(void *owner);
extern float func_08AAF2AC(void *motion);
extern int func_08A65C64(void *queue, float rate);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A69158(CrushEvent *self)
{
    float rate;
    if (!self->open) {
        return -1;
    }
    rate = func_08AAF2AC(func_08AB327C(self->owner));
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 9);
    func_08A65B50(self->queue, 0xA);
    func_08A65C64(self->queue, rate);
    func_08A65BAC(self->queue, 1);
    func_08A65BAC(self->queue, 6);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0xB);
    func_08A65B50(self->queue, 0x1E);
    func_08A65C08(self->queue, 1);
    func_08A65B50(self->queue, 0x1A);
    func_08A65BAC(self->queue, 1);
    func_08A65BAC(self->queue, 6);
    func_08A65AFC(self->queue);
    return 0;
}
