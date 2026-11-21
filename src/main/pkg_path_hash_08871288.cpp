/* The C library's character class table; (_ctype_ + 1)[c] & 1 means "upper case". */
extern "C" const char D_089CF590[];

struct PathHash {
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

/* pkg_path_hash: CRC-32 of a package path, the key of the PACKAGE_INFO index.
 * Initial value and final xor 0xFFFFFFFF; case is ignored (letters folded to lower case). */
unsigned int func_08871288(const char *path)
{
    unsigned int crc = 0xFFFFFFFF;
    int c;

    while ((c = *path++) != 0) {
        c = PathHash::lower(c);
        crc ^= c << 24;
        for (int bit = 0; bit < 8; bit++) {
            crc = PathHash::step(crc);
        }
    }
    return ~crc;
}

}
