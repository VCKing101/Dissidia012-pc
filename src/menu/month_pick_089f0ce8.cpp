/* Screen0B6A0: a random month (1..12) out of the months set in the mask
   of day `index` (1..0x20) at +0x32FC; 1 when there is none. */
struct MonthWork29 {
    char pad_0000[0x32FC];
    unsigned short masks[0x21];
};

struct MonthScreen29 {
    char pad_000[0x14C];
    MonthWork29 *work;
};

extern "C" int D_09B6DA54;

/* The menu's random generator (MSVC rand() constants), inlined. */
struct Random29 {
    static int Next() {
        D_09B6DA54 = D_09B6DA54 * 0x343FD + 0x269EC3;
        return (D_09B6DA54 >> 16) & 0x7FFF;
    }
    static int Range(int low, int high) {
        return Next() % (high - low + 1) + low;
    }
};

extern "C" int func_089F0CE8(MonthScreen29 *self, char index)
{
    char months[16];
    int result = 1;
    int count = 0;

    if (index > 0 && index < 0x21) {
        for (int month = 1; month < 13; month++) {
            if (self->work->masks[index] & (1 << month)) {
                months[count] = month;
                count++;
            }
        }
        if (count != 0) {
            result = months[Random29::Range(0, count - 1)];
        }
    }
    return result;
}
