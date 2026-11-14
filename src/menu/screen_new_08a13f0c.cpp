/* `new Screen15A88(owner)`: a 0xF70-byte screen allocated from the menu heap
   (func_08A24090) and built by func_08A13DB0. Operator new and the
   constructor are in-class inlines, so this is a real new-expression
   (the null result kept in an s-register). */
extern "C" void *func_08A24090(int size);
extern "C" void func_08A13DB0(void *screen, void *owner);

struct Screen15A88New {
    char data[0xF70];
    static void *operator new(unsigned int size) { return func_08A24090(size); }
    Screen15A88New(void *owner) { func_08A13DB0(this, owner); }
};

extern "C" Screen15A88New *func_08A13F0C(void *owner)
{
    return new Screen15A88New(owner);
}
