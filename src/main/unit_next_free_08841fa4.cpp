/* Returns the next unit after u whose kind is 0, or 0. */
struct Unit08841FA4 {
    char pad_000[0x542];
    unsigned char kind;

    static unsigned char is_free(Unit08841FA4 *u) { return u->kind == 0; }
};

extern "C" {

Unit08841FA4 *func_08841F84(Unit08841FA4 *u);

Unit08841FA4 *func_08841FA4(Unit08841FA4 *u)
{
    for (;;) {
        u = func_08841F84(u);
        if (u == 0 || Unit08841FA4::is_free(u) == 1) {
            break;
        }
    }
    return u;
}

}
