/* Per-state handler for an active prompt: returns 1 when none is active, otherwise acts on the input state and returns 0. */
typedef struct S { char p0[0xEC3]; unsigned char active; } S;
extern int func_089C5868(void);
extern int func_088AE148(S *self, int a);
extern void func_089A0190(int a, int b);
extern void func_089C5898(int a);
extern void func_089C5878(void);
int func_088AD10C(S *self, int a)
{
    if (self->active == 0) {
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
        if (func_088AE148(self, a) != 0) {
            self->active = 0;
        }
        break;
    default:
        break;
    }
    return 0;
}
