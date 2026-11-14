/* Returns the next unit after u whose kind is 1, or 0. */
struct Unit08841FF4 {
    char pad_000[0x542];
    unsigned char kind;

    static unsigned char is_kind1(Unit08841FF4 *u) { return u->kind == 1; }
};

extern "C" {

Unit08841FF4 *func_08841F84(Unit08841FF4 *u);

Unit08841FF4 *func_08841FF4(Unit08841FF4 *u)
{
    for (;;) {
        u = func_08841F84(u);
        if (u == 0 || Unit08841FF4::is_kind1(u) == 1) {
            break;
        }
    }
    return u;
}

}
