/* If flag bit 0 is set and the queue accepts, calls a hook, releases 0x15 held cells and moves to state 3. Returns 0. */
struct St { char pad0[0x884]; int cells[1]; char pad1[0x984 - 0x888]; int state; };
struct Owner { char pad[0x138]; struct St *st; };
extern int D_089D69E8;
extern int D_08B01CF8;
extern int func_08885608(int, int);
extern void func_08AF7F1C(int);
extern void func_0887BA5C(int);
int func_08B23778(struct Owner *self) {
    int i;
    int off;
    int zero = 0;
    if ((*(unsigned *)((char *)self->st + 0x988) & 1) == 0) {
        return 0;
    }
    if (func_08885608(D_089D69E8, zero) == 0) {
        return 0;
    }
    {
        func_08AF7F1C(D_08B01CF8);
        i = 0; off = (int)self->st; off = 0;
        do {
            int v = *(int *)((char *)self->st + off + 0x884);
            if (v != 0) {
                func_0887BA5C(v);
                *(int *)((char *)self->st + off + 0x884) = 0;
            }
            i++;
            off += 4;
        } while (i < 0x15);
        self->st->state = 3;
    }
    return 0;
}
