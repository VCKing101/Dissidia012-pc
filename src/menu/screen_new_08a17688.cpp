/* `new Screen17540(owner)`: a 0xD90-byte screen allocated from the menu heap
   (func_08A24090) and built by func_08A17540. Operator new and the
   constructor are in-class inlines, so this is a real new-expression
   (the null result kept in an s-register). */
extern "C" void *func_08A24090(int size);
extern "C" void func_08A17540(void *screen, void *owner);

struct Screen17540New {
    char data[0xD90];
    static void *operator new(unsigned int size) { return func_08A24090(size); }
    Screen17540New(void *owner) { func_08A17540(this, owner); }
};

extern "C" Screen17540New *func_08A17688(void *owner)
{
    return new Screen17540New(owner);
}
