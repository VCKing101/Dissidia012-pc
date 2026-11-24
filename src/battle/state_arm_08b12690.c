/* If func_088FEA4C reports nothing, signals D_089D5FB4 and sets the linked object's state word to 4. */
typedef struct Linked { char pad_00[0x88C]; int state; } Linked;
typedef struct ArmSelf { char pad_00[8]; Linked *linked; } ArmSelf;
extern int D_089D5FB4;
extern int func_088FEA4C(void);
extern void func_088705F0(int v);
void func_08B12690(ArmSelf *self)
{
    int busy = func_088FEA4C();
    if (busy) {
        return;
    } else {
        func_088705F0(D_089D5FB4);
        self->linked->state = 4;
    }
}
