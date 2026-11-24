/* If func_08B3C1EC(self) is non-zero, stores 8 into the word at self[0xC4]. */
typedef struct S { char pad[0xC4]; int *p; } S;
extern int func_08B3C1EC(S *self);
void func_08B36BCC(S *self)
{
    if (!func_08B3C1EC(self)) return;
    *self->p = 8;
}
