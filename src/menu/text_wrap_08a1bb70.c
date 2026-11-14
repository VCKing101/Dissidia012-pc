/* Wraps a long text (more than 0x19 characters, func_08A1BBE8) in place:
   moves the characters up and puts a line feed after every 25th one. */
extern int func_08A1BBE8(void *self, unsigned short *text);

void func_08A1BB70(void *self, unsigned short *text)
{
    unsigned short *dst;
    int i;

    if (func_08A1BBE8(self, text) >= 0x1A) {
        i = 0x4A;
        dst = &text[0x4C];
        dst[1] = 0;
        do {
            *dst = text[i];
            dst--;
            if (i % 25 == 0) {
                *dst = 10;
                dst--;
            }
            i--;
        } while (i > 0x18);
    }
}
