/* Once the scene clock passes the event's start time, queues the motion
 * speed block (commands 7 and 8) with the current motion's speed; -1 when
 * the event is closed. */
typedef struct SpeedEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} SpeedEvent;

extern float func_08A66820(SpeedEvent *self);
extern float func_08A6677C(SpeedEvent *self);
extern void *func_08AB327C(void *owner);
extern float func_08AAF344(void *motion);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C64(void *queue, float rate);

int func_08A66A08(SpeedEvent *self)
{
    float start;
    float speed;
    if (!self->open) {
        return -1;
    }
    start = func_08A66820(self);
    if (!(func_08A6677C(self) < start)) {
        speed = func_08AAF344(func_08AB327C(self->owner));
        func_08A65A84(self->queue);
        func_08A65AAC(self->queue);
        func_08A65B50(self->queue, 7);
        func_08A65C64(self->queue, speed);
        func_08A65B50(self->queue, 8);
        func_08A65BAC(self->queue, 0);
        func_08A65AFC(self->queue);
    }
    return 0;
}
