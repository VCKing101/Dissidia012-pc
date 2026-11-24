/* After func_08B49754, calls func_08922ADC on each of the 32 0x24-byte entries of the list at self[0]. */
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" void func_08B49754(Owner *self);
extern "C" void func_08922ADC(char *entry);
extern "C" void func_08B4869C(Owner *self)
{
    func_08B49754(self);
    for (int i = 0, off = 0; i < 0x20; i++, off += 0x24) {
        func_08922ADC(self->list->items + off);
    }
}
