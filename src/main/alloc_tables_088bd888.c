// Allocates a 0x26E0-byte block into D_089D73D0 and a second block from it into D_089D73D4.
extern int func_088820D8(int);
extern int func_0885C40C(int, int, const void *, int);
extern int D_089D73D0, D_089D73D4;
extern char D_089CBA28[];
void func_088BD888(void)
{
    int size = 0x26E0;
    D_089D73D0 = func_088820D8(size);
    D_089D73D4 = func_0885C40C(D_089D73D0, size, D_089CBA28, 4);
}
