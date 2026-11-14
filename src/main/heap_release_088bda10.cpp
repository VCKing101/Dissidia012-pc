/* Releases the heap at 0x089D73D8 through its third virtual and forgets it. */
struct Heap088BDA10 {
    char data[0x24];
    virtual void v0();
    virtual void v1();
    virtual void release();
};
extern "C" {
extern Heap088BDA10 *D_089D73D8;
void func_088BDA10(void)
{
    if (D_089D73D8 != 0) {
        D_089D73D8->release();
        D_089D73D8 = 0;
    }
}
}
