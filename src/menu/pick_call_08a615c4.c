extern void func_088D9CD8(int handle);
extern void func_08A61618(int self);
extern int func_08A61810(int self);

void func_08A615C4(int self)
{
    int index;
    func_08A61618(self);
    index = func_08A61810(self);
    index = index + 0xB;
    if (index >= 0xB && index < 0x16) {
        func_088D9CD8(*(int *)(self + index * 4 + 0x40));
    }
}
