struct Placement60E4C {
    char pad_000[0x34];
    unsigned int flags;

    static void set_flags(Placement60E4C *placement, unsigned int bits)
    {
        placement->flags |= bits;
    }
};

struct Model60E4C {
    char pad_000[0x48];
    Placement60E4C placement;
};

struct Fighter60E4C {
    char pad_000[0x314];
    Model60E4C *model;
};

extern "C" {

void func_08A60E4C(Fighter60E4C *fighter)
{
    Placement60E4C::set_flags(&fighter->model->placement, 0x10);
}

void func_08A60E64(Fighter60E4C *fighter)
{
    Placement60E4C::set_flags(&fighter->model->placement, 0x20);
}

void func_08A60E7C(Fighter60E4C *fighter)
{
    Placement60E4C::set_flags(&fighter->model->placement, 0x40);
}

}
