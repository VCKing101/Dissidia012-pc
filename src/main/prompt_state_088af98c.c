/* Per-state handler for an active prompt on the owner's sub-object: returns 1 when none is active, otherwise acts on the input state and returns 0. */
typedef struct Sub { char p0[0x2B54]; unsigned char active; } Sub;
typedef struct S { char p0[0xC]; Sub *sub; } S;
extern int func_089C5868(void);
extern int func_088AF868(S *self, int a);
extern void func_089A0190(int a, int b);
extern void func_089C5898(int a);
extern void func_089C5878(void);
int func_088AF98C(S *self, int a)
{
    if (self->sub->active == 0) {
        return 1;
    }
    switch (func_089C5868()) {
    case 1:
        break;
    case 2:
        func_089A0190(4, 0);
        func_089C5898(2);
        break;
    case 3:
        func_089C5878();
        break;
    case 4:
        break;
    case 0:
        if (func_088AF868(self, a) != 0) {
            self->sub->active = 0;
        }
        break;
    default:
        break;
    }
    return 0;
}
