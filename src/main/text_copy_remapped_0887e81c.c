/* Copy a 0-terminated UTF-16 string into a buffer of size bytes, replacing the characters the remap table at 0x089D6988 lists (0xFFFF ends the table). */
typedef struct Remap0887E81C {
    unsigned short from;
    unsigned short to;
} Remap0887E81C;
extern Remap0887E81C D_089D6988[];
void func_0887E81C(const unsigned short *src, unsigned short *dst, unsigned int size)
{
    int n = 0;
    unsigned int count = size >> 1;
    unsigned int room = 1;
    Remap0887E81C *e;
    Remap0887E81C *table = D_089D6988;
    unsigned int end = 0xFFFF;

    room = room < count;
    while (1) {
        if (!room) {
            *dst = 0;
            break;
        }
        *dst = *src;
        n += 2;
        if (*src == 0) {
            break;
        }
        room = n / 2 + 1 < count;
        e = table;
        while (1) {
            unsigned int f = e->from;
            if (f == end) {
                break;
            }
            if (f == *dst) {
                *dst = e->to;
                break;
            }
            e++;
        }
        src++;
        dst++;
    }
}
