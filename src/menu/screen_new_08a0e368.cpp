/* `new Screen11510(owner)`: a 0x1330-byte screen allocated from the menu heap
   (func_08A24090) and built by func_08A0E220. Operator new and the
   constructor are in-class inlines, so this is a real new-expression
   (the null result kept in an s-register). */
extern "C" void *func_08A24090(int size);
extern "C" void func_08A0E220(void *screen, void *owner);

struct Screen11510New {
    char data[0x1330];
    static void *operator new(unsigned int size) { return func_08A24090(size); }
    Screen11510New(void *owner) { func_08A0E220(this, owner); }
};

extern "C" Screen11510New *func_08A0E368(void *owner)
{
    return new Screen11510New(owner);
}
