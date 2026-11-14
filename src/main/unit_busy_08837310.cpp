/* 1 when the unit is held (flag 0x20000) or otherwise busy. */
struct Unit08837310 {
    char pad_000[0x384];
    unsigned int flags;

    static bool held(Unit08837310 *u) { return (u->flags & 0x20000) != 0; }
};

extern "C" {

int func_08837360(Unit08837310 *u);

unsigned char func_08837310(Unit08837310 *u)
{
    int result = 0;
    if (Unit08837310::held(u) || func_08837360(u) != 0) {
        result = 1;
    }
    return result;
}

}
