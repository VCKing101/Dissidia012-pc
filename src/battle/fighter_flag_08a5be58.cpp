struct Fighter5BE58 {
    char pad_000[0x384];
    unsigned int flags;

    static bool has_flag_400(Fighter5BE58 *fighter)
    {
        return (fighter->flags & 0x400) != 0;
    }
};

extern "C" {

int func_08A5BE58(void *self, Fighter5BE58 *fighter)
{
    return Fighter5BE58::has_flag_400(fighter) != 0;
}

}
