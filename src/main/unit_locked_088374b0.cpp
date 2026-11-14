/* 1 when the unit is guarding, in a locked motion, stunned, or of kind 0x12. */
struct Info088374B0 {
    int kind;
};

struct Unit088374B0 {
    char pad_000[0x3A4];
    Info088374B0 *info;

    static bool kind_is(Unit088374B0 *u, int kind) { return u->info->kind == kind; }
};

extern "C" {

int func_088373C8(Unit088374B0 *u);
int func_08837584(Unit088374B0 *u);
int func_0883752C(Unit088374B0 *u);

unsigned char func_088374B0(Unit088374B0 *u)
{
    return (func_088373C8(u) != 0 || func_08837584(u) != 0 || func_0883752C(u) != 0 ||
            Unit088374B0::kind_is(u, 0x12)) ? 1 : 0;
}

}
