/* The C library's character class table; (_ctype_ + 1)[c] & 1 means "upper case". */
extern "C" const char D_089CF590[];

struct NameHash {
    /* tolower() for the hash: A-Z -> a-z, everything else unchanged. */
    static int lower(int c) {
        if ((D_089CF590 + 1)[c] & 1) {
            return c + 0x20;
        }
        return c;
    }
    /* One bit of an MSB-first CRC-32 (polynomial 0x04C11DB7). */
    static unsigned int step(unsigned int crc) {
        if (crc & 0x80000000) {
            return (crc << 1) ^ 0x04C11DB7;
        }
        return crc << 1;
    }
};

extern "C" {

extern char D_08B634EF[];              /* the name buffer (reused) */
extern char D_089C8ED8[];              /* the hex format */
int func_0893476C(char *dst, const char *format, ...);   /* sprintf */

/* edat_name: the file name of a DLC .edat, the case-folded CRC-32 of its path in hex
 * (same hash as pkg_path_hash). Returns the shared buffer. */
char *func_08861B84(const char *path)
{
    unsigned int crc = 0xFFFFFFFF;
    int c;

    while ((c = *path++) != 0) {
        c = NameHash::lower(c);
        crc ^= c << 24;
        for (int bit = 0; bit < 8; bit++) {
            crc = NameHash::step(crc);
        }
    }
    func_0893476C(D_08B634EF, D_089C8ED8, ~crc);
    return D_08B634EF;
}

}
