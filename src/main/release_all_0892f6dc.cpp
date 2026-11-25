// Releases the one 0x10C-byte sub-block, then the two parts of the object.
extern "C" void func_0892ECAC(void *);
extern "C" void func_0892FD70(void *);
extern "C" void func_0892FCA8(void *);
extern "C" void func_0892F6DC(char *self) {
    for (int i = 0, *p = (int *)(self + 0x3C); i < 1; i++, p += 0x43) { func_0892ECAC(p); }
    func_0892FD70(self);
    func_0892FCA8(self);
}
