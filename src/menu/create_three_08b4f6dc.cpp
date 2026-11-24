/* After func_08B4F754(self), for three slots self+0x30 = func_088DAA68(self->0x2C); then func_08B4F7A8(self) and func_088D3CF0(0, self+0x30, func_08B4FD98). */
extern "C" void func_08B4F754(int *self);
extern "C" int func_088DAA68(int src);
extern "C" void func_08B4F7A8(int *self);
extern "C" void func_088D3CF0(int z, int *slots, void *tag);
extern "C" char D_08B4FD98[];
extern "C" void func_08B4F6DC(int *self)
{
    func_08B4F754(self);
    for (int i = 0, *p = self; i < 3; i++, p++) { p[0x30 / 4] = func_088DAA68(self[0x2C / 4]); }
    func_08B4F7A8(self);
    func_088D3CF0(0, self + 0x30 / 4, D_08B4FD98);
}
