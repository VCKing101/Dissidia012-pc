/* Queues the fighter's lunge script (rate from the current motion, a
 * random lunge 0x27 or 0x28, then the holder's time) once the event has
 * started before the holder's time; otherwise only clears the queue (-1
 * closed, -2 no fighter, -3 no holder). */
typedef struct CrushFighter {
    unsigned char pad_000[0x39C];
    void *holder;
} CrushFighter;

typedef struct CrushInfo {
    unsigned char pad_00[4];
    CrushFighter *fighter;
} CrushInfo;

typedef struct CrushEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
    unsigned char pad_68[8];
    void *queue;
} CrushEvent;

extern CrushInfo *func_08AB3264(void *owner);
extern float func_08A66820(void *self);
extern float func_0884E474(void *holder);
extern void *func_08AB327C(void *owner);
extern float func_08AAF474(void *motion);
extern int func_08AA5748(void *random);
extern void *D_08AA5828;
extern int func_08A65C64(void *queue, float rate);
extern void func_08A65A84(void *queue);
extern void func_08A65AAC(void *queue);
extern void func_08A65AFC(void *queue);
extern int func_08A65B50(void *queue, int value);
extern int func_08A65BAC(void *queue, int value);
extern int func_08A65C08(void *queue, int value);

int func_08A6A190(CrushEvent *self)
{
    CrushFighter *fighter;
    void *holder;
    float start;
    float time;
    float rate;
    if (!self->open) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    holder = fighter->holder;
    if (holder == 0) {
        return -3;
    }
    start = func_08A66820(self);
    time = func_0884E474(holder);
    if (!(start <= time)) {
        rate = func_08AAF474(func_08AB327C(self->owner));
        func_08A65A84(self->queue);
        func_08A65AAC(self->queue);
        func_08A65B50(self->queue, 2);
        func_08A65C64(self->queue, rate);
        if (func_08AA5748(D_08AA5828) % 2 == 0) {
            func_08A65B50(self->queue, 0x27);
        } else {
            func_08A65B50(self->queue, 0x28);
        }
        func_08A65B50(self->queue, 5);
        func_08A65C64(self->queue, time);
        func_08A65AFC(self->queue);
    } else {
        func_08A65A84(self->queue);
    }
    return 0;
}
