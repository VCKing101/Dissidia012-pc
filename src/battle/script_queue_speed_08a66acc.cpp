/* Queues the command blocks that replay the owner's motion at its current
 * speed (-1 closed); the event stays in the frame across the calls. */
struct SpeedEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
};

template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {

extern void *func_08AB327C(void *owner);
extern float func_08AAFFC8(void *motion);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);
extern int func_08A65C64(void *queue, float rate);

int func_08A66ACC(SpeedEvent *self)
{
    if (!self->open) {
        return -1;
    }
    Kept<SpeedEvent *> saved(self);
    float speed = func_08AAFFC8(func_08AB327C(saved.value->owner));
    func_08A65A84(saved.value->queue);
    func_08A65AAC(saved.value->queue);
    func_08A65B50(saved.value->queue, 0xB);
    func_08A65B50(saved.value->queue, 6);
    func_08A65B50(saved.value->queue, 2);
    func_08A65C64(saved.value->queue, speed);
    func_08A65BAC(saved.value->queue, 1);
    func_08A65AFC(saved.value->queue);
    func_08A65AAC(saved.value->queue);
    func_08A65B50(saved.value->queue, 0);
    func_08A65C08(saved.value->queue, 0);
    func_08A65AFC(saved.value->queue);
    return 0;
}

}
