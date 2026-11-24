/* Calls func_0892AC2C(self), clears self+0x28 and self+0x2C, then the one-element array at self+0x30. */
extern "C" void func_0892AC2C(int *self);
extern "C" void func_08B5232C(int *self)
{
    func_0892AC2C(self);
    self[0x28 / 4] = 0;
    self[0x2C / 4] = 0;
    for (int i = 0; i < 1; i++, self++) { self[0x30 / 4] = 0; }
}
