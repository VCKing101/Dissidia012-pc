/* 1 when the source's id is not in the owner's bit set and its timer has run out. */
struct Owner0880ACBC {
    int field_00;
    unsigned int bits[16];
    float timer;

    static bool has(unsigned int *bits, unsigned int id)
    {
        return (bits[id >> 5] & (1 << (id & 0x1F))) != 0;
    }
};

struct Source0880ACBC {
    char pad_00[0x26];
    short id;
};

extern "C" {

bool func_0880ACBC(Owner0880ACBC *self, Source0880ACBC *src)
{
    if (Owner0880ACBC::has(self->bits, src->id)) {
        return false;
    }
    if (!(self->timer <= 0.0f)) {
        return false;
    }
    return true;
}

}
