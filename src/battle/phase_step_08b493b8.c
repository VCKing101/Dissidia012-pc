// By the phase word at +0: 1 runs func_08B49418, 2 runs func_08B49418 then func_08B4946C.
extern void func_08B49418(void *);
extern void func_08B4946C(void *);
typedef struct { int phase; } Obj08B493B8;
void func_08B493B8(Obj08B493B8 *self)
{
    switch (self->phase) {
    case 1:
        func_08B49418(self);
        break;
    case 2:
        func_08B49418(self);
        func_08B4946C(self);
        break;
    }
}
