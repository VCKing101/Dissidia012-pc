// Sets the state word at +8 to 2 unless it is 3.
typedef struct { char pad[8]; int state; } Obj08B44CFC;
void func_08B44CFC(Obj08B44CFC *self)
{
    int cur = self->state;
    int three = 3;
    if (cur == three) {
        return;
    }
    self->state = 2;
}
