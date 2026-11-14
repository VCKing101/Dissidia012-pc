/* `new Screen19638(owner)`: a 0x17C0-byte screen allocated from the menu heap
   (func_08A24090) and built by func_08A19638. Operator new and the
   constructor are in-class inlines, so this is a real new-expression
   (the null result kept in an s-register). */
extern "C" void *func_08A24090(int size);
extern "C" void func_08A19638(void *screen, void *owner);

struct Screen19638New {
    char data[0x17C0];
    static void *operator new(unsigned int size) { return func_08A24090(size); }
    Screen19638New(void *owner) { func_08A19638(this, owner); }
};

extern "C" Screen19638New *func_08A19774(void *owner)
{
    return new Screen19638New(owner);
}
