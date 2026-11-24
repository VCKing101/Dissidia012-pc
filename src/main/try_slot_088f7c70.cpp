// If there is room, start the numbered 0x1270-byte entry; return whether there was.
extern "C" int func_088F7D0C(int self);
extern "C" void func_088F5188(int entry);
extern "C" bool func_088F7C70(int self, int index)
{
    int r = func_088F7D0C(self);
    if (r == 1) {
        func_088F5188(self + index * 0x1270 + 0x14C80);
        return true;
    }
    return false;
}
