typedef struct Pager { char pad_00[0x54]; int pages[5]; int index; } Pager;
extern void func_08861C48(int page);
void func_088625AC(Pager *self)
{
    self->index = 0;
    func_08861C48(self->pages[self->index]);
}
