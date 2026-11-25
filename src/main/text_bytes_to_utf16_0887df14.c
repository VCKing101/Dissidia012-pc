/* Convert a Shift-JIS style byte string to UTF-16 in a buffer of count characters: ASCII as is, two-byte pairs through the table at 0x089D6964 (0x3F when missing), 0 terminator. */
typedef struct Pair0887DF14 {
    char a;
    char b;
    unsigned short pad;
    unsigned short code;
} Pair0887DF14;
extern Pair0887DF14 D_089D6964[];
void func_0887DF14(void *unused, unsigned short *dst, unsigned int count, const unsigned char *src)
{
    unsigned short *out = dst;
    unsigned int pos = 0;
    unsigned int prev = 0;
    unsigned int i = 1;
    Pair0887DF14 *table = D_089D6964;
    unsigned int q = 0x3F;
    int room = i < count;

    while (1) {
        int c;
        unsigned int c2;
        Pair0887DF14 *e;
        if (!room) {
            break;
        }
        c = *(const unsigned char *)(pos + (unsigned int)src);
        pos++;
        if (c == 0) {
            break;
        }
        if (c < 0x80) {
            *out = c & 0xFF;
        } else {
            c2 = *(const unsigned char *)(pos + (unsigned int)src);
            pos++;
            e = table;
            while (1) {
                unsigned int code = e->code;
                if (code == 0) {
                    *out = q;
                    break;
                }
                if ((unsigned char)e->a == c && (unsigned char)e->b == c2) {
                    *out = code;
                    break;
                }
                e = (Pair0887DF14 *)((char *)e + 6);
            }
        }
        prev = i;
        i = prev + 1;
        out++;
        room = i < count;
    }
    if (prev < count) {
        *out = 0;
    }
}
