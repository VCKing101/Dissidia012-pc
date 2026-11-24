/* When the widgets are ready, waits out the pending sound, then refreshes the screen and moves to state 6. */
extern int func_08B4E7D4(void *a);
extern int func_088A751C(void);
extern void func_08B46698(void *a, int b);
extern void func_0892D380(void *a, void *b, int c, int d);
extern void func_08B342D4(void *a, int b);

typedef struct W36FA0 { void *w0; char pad04[0x10]; void *w14; char pad18[4]; void *w1C; char pad20[4]; void *w24; } W36FA0;
typedef struct B36FA0 { int state; char pad04[8]; void *wC; void *w10; } B36FA0;
typedef struct H36FA0 { int pad0; int pending; } H36FA0;
typedef struct S36FA0 { char pad[0xC4]; B36FA0 *b; W36FA0 *w; int padCC; H36FA0 *h; } S36FA0;

void func_08B36FA0(S36FA0 *self)
{
    if (func_08B4E7D4(self->w->w24) == 0) {
        return;
    }
    if (self->h->pending != 0) {
        if (func_088A751C() != 0) {
            return;
        }
        self->h->pending = 0;
    }
    func_08B46698(self->b->w10, 1);
    func_0892D380(self->w->w1C, self->w->w0, 0xB, 1);
    func_08B342D4(self->b->wC, 1);
    self->b->state = 6;
}
