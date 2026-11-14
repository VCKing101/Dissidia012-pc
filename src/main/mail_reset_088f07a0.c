/* Resets the mail state globals and clears the 16-byte mail buffer. */
extern short D_089D7F14;
extern short D_089D7F16;
extern short D_089D7F18;
extern unsigned char D_089D7F1A;
extern unsigned char D_089D7F1B;
extern char D_09B698C8[];
extern void func_08935828(void *dst, int value, int size);

void func_088F07A0(void)
{
    D_089D7F14 = 0;
    D_089D7F16 = 1;
    D_089D7F18 = 0;
    D_089D7F1A = 0;
    D_089D7F1B = 0;
    func_08935828(D_09B698C8, 0, 0x10);
}
