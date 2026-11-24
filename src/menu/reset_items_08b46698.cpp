/* Calls func_08925424(item, 0) for each item id in the owner list; when the flag is set, then runs func_08B477AC on the owner. */
struct L { char pad[0x2C]; int items[70]; int count; };
struct Owner { char pad[0x18]; L *list; };
extern "C" void func_08925424(int id, int v);
extern "C" void func_08B477AC(Owner *self);
extern "C" void func_08B46698(Owner *self, unsigned char flag)
{
    for (int i = 0, off = 0; i < self->list->count; i++, off += 4) {
        func_08925424(((int *)((char *)self->list + off))[0x2C / 4], 0);
    }
    if (flag) {
        func_08B477AC(self);
    }
}
