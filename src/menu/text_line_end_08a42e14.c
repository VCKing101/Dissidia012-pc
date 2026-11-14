/* Returns the character after the first line break, skipping control codes 0x11-0x1F with their arguments; 0 if none. */
extern int func_0887E7F4(int code);

unsigned short *func_08A42E14(unsigned short *text)
{
    int found = 0;
    int c = *text;
    while (c != 0) {
        if (c == 10) {
            found = 1;
        }
        if (c > 0x10 && c < 0x20) {
            text += func_0887E7F4(*text);
        }
        text++;
        if (found) {
            return text;
        }
        c = *text;
    }
    return 0;
}
