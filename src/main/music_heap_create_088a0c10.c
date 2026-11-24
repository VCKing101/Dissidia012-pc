/* Allocates the music memory block and creates its heap; returns whether
   the block was allocated. */
extern int D_089D6C78;
extern char D_09B4CF80[];

extern int func_0888204C(int size);
extern void func_08867310(void *heap, int block, unsigned int size, int align);

int func_088A0C10(void)
{
    D_089D6C78 = func_0888204C(0x237000);
    if (D_089D6C78 == 0) {
        return 0;
    }
    func_08867310(D_09B4CF80, D_089D6C78, 0x237000, 0x40);
    return 1;
}
