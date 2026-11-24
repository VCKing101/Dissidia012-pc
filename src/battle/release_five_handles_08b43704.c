// Releases up to five resource handles stored as shorts in an attached table.
extern void *func_08899300(void);
extern void func_088981B4(void *, int, int, int);
typedef struct { int pad; short h[5]; } T;
typedef struct { int pad; T *t; } O;
void func_08B43704(O *self) {
    void *m = func_08899300();
    if (self->t->h[0] >= 0) func_088981B4(m, self->t->h[0], 0, 1);
    if (self->t->h[1] >= 0) func_088981B4(m, self->t->h[1], 0, 1);
    if (self->t->h[2] >= 0) func_088981B4(m, self->t->h[2], 0, 1);
    if (self->t->h[3] >= 0) func_088981B4(m, self->t->h[3], 0, 1);
    if (self->t->h[4] >= 0) func_088981B4(m, self->t->h[4], 0, 1);
}
