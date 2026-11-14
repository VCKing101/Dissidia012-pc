/* Queues the two command blocks that replay the owner's motion at its
 * current rate (-1 closed). */
typedef struct RateEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} RateEvent;

extern void *func_08AB327C(void *owner);
extern float func_08AAF2AC(void *motion);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);
extern int func_08A65C64(void *queue, float rate);

int func_08A6905C(RateEvent *self)
{
    float rate;
    if (!self->open) {
        return -1;
    }
    rate = func_08AAF2AC(func_08AB327C(self->owner));
    func_08A65A84(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 9);
    func_08A65B50(self->queue, 0xC);
    func_08A65C64(self->queue, rate);
    func_08A65B50(self->queue, 6);
    func_08A65BAC(self->queue, 1);
    func_08A65BAC(self->queue, 8);
    func_08A65AFC(self->queue);
    func_08A65AAC(self->queue);
    func_08A65B50(self->queue, 0x1E);
    func_08A65C08(self->queue, 1);
    func_08A65B50(self->queue, 0x1A);
    func_08A65BAC(self->queue, 1);
    func_08A65BAC(self->queue, 8);
    func_08A65AFC(self->queue);
    return 0;
}
