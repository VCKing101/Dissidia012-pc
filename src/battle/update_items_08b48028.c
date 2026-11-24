// Runs func_08B493B8 on +0x58 and func_08B47214 on +0xB0, then func_08B475E4 on each of the items (stride 0x4C from +0xBC); count at +0x4E4.
extern void func_08B493B8(void *);
extern void func_08B47214(void *);
extern void func_08B475E4(void *);
typedef struct { char data[0x4C]; } Item08B48028;
typedef struct { char pad0[0x58]; char a[0x58]; char b[0xC]; Item08B48028 items[14]; int count; } Obj08B48028;
void func_08B48028(Obj08B48028 *self)
{
    int i;
    func_08B493B8(self->a);
    func_08B47214(self->b);
    for (i = 0; i < self->count; i++) {
        func_08B475E4(&self->items[i]);
    }
}
