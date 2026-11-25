/* Runs the current queue entry's callback: -1 when past the end, -2 without an entry, -3 without a callback; when it returns 1, completes the cursor and sets state 4. */
typedef int (*Cb088D7A2C)(void);
typedef struct O088D7A2C { char pad0[0x10]; int state; char pad1[0x24 - 0x14]; char cur[0x38]; char list[1]; } O088D7A2C;
unsigned int func_088D82F0(void *c);
unsigned int func_088D93E0(void *l);
void *func_088D9418(void *l, unsigned int i);
Cb088D7A2C func_088D8F00(void *e);
void func_088D83D8(void *c);
int func_088D7A2C(O088D7A2C *self)
{
    char *c = self->cur;
    unsigned int i = func_088D82F0(c);
    char *l = self->list;
    void *e;
    Cb088D7A2C fn;
    if (!(i < func_088D93E0(l))) {
        return -1;
    }
    e = func_088D9418(l, i);
    if (e == 0) {
        return -2;
    }
    fn = func_088D8F00(e);
    if (fn == 0) {
        return -3;
    }
    if (fn() == 1) {
        func_088D83D8(c);
        self->state = 4;
    }
    return 0;
}
