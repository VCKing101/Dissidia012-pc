/* Returns the length of a zero-terminated array of 16-bit values. */
struct Len08935E90 {
    static int len(unsigned short *s) {
        unsigned short *p = s;
        while (*p != 0) { p++; }
        return p - s;
    }
};
extern "C" int func_08935E90(unsigned short *s) {
    return Len08935E90::len(s);
}
