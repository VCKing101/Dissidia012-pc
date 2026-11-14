/* Finds the alternate entry of a "P_" entry: the "_A" variant's base entry,
   or else the first entry of its group. */
struct Info08844FE4 {
    char pad_00[6];
    signed char group;
    char pad_07[0x24 - 7];
    char name[1];
};

struct Name08844FE4 {
    static unsigned char is(const char *s, int a, int b)
    {
        int r = 0;
        if (s[0] == a && s[1] == b) {
            r = 1;
        }
        return r;
    }

    static unsigned char ends(const char *end, int a, int b)
    {
        int r = 0;
        if (end[-2] == a && end[-1] == b) {
            r = 1;
        }
        return r;
    }
};

extern "C" {

int func_089359E8(const char *s);
Info08844FE4 *func_08847130(int group, int a);
Info08844FE4 *func_088470DC(int group, int a, int b);

Info08844FE4 *func_08844FE4(Info08844FE4 *self)
{
    char *name = self->name;
    Info08844FE4 *alt = 0;
    if (Name08844FE4::is(name, 'P', '_')) {
        name += func_089359E8(name);
        if (Name08844FE4::ends(name, '_', 'A')) {
            alt = func_08847130(self->group, 1);
        } else {
            alt = func_088470DC(self->group, 1, 0);
        }
    }
    return alt;
}

}
