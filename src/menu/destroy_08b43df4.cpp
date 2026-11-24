/* Resets the vtable word at +0xC to the base table and, when bit 0 of the flag is set, frees the object with func_08B43E44. */
extern "C" char D_08B44E90[];
extern "C" void func_08B43E44(int *self);
struct Dtor {
    static void run(int *self, int flag)
    {
        if (self) {
            self[3] = (int)D_08B44E90;
            if (flag & 1) {
                func_08B43E44(self);
            }
        }
    }
};
extern "C" void func_08B43DF4(int *self, int flag)
{
    Dtor::run(self, flag);
}
