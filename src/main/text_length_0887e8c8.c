/* Byte length of a UTF-16 text up to its end, a stop code, or a 0x1F tag marker followed by tag, skipping the arguments of control codes 0x11-0x1F; clamped to a buffer of size characters. */
extern int func_0887E7F4(int code);
unsigned int func_0887E8C8(const unsigned short *text, int size, unsigned short stop, unsigned short tag)
{
    const unsigned short *p = text;
    const unsigned short *base = text;
    unsigned int length;
    int marker;

    if (p == 0) {
        return 0;
    }
    marker = 0x1F;
    for (; *p != 0;) {
        if (stop != 0 && *p == stop) {
            break;
        }
        if (tag != 0 && *p == marker && p[1] == tag) {
            break;
        }
        if (*p > 0x10 && *p < 0x20) {
            p += func_0887E7F4(*p);
        }
        p++;
    }
    length = (const char *)p - (const char *)base;
    if ((unsigned int)(size - 1) * 2 < length) {
        length = size - 2;
    }
    return length;
}
