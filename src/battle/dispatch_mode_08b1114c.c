// Dispatches on the word at +4 of b (0..3) to func_08B14794 / 08B14828 / 08B148B4 / 08B14920 with the pointer at +0x38; returns 0.
extern void func_08B14794(void *);
extern void func_08B14828(void *);
extern void func_08B148B4(void *);
extern void func_08B14920(void *);
typedef struct { char pad[0x38]; void *target; } Obj08B1114C;
int func_08B1114C(Obj08B1114C *self, int *b)
{
    int *pi = b + 1;
    switch (*pi) {
    case 0:
        func_08B14794(self->target);
        break;
    case 1:
        func_08B14828(self->target);
        break;
    case 2:
        func_08B148B4(self->target);
        break;
    case 3:
        func_08B14920(self->target);
        break;
    }
    return 0;
}
