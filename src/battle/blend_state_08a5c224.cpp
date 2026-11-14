struct BlendState5C224 {
    char pad_000[0x2C];
    float weight;
    float time;
    unsigned int flags;

    static bool is_active(BlendState5C224 *blend)
    {
        return (blend->flags & 8) != 0;
    }

    static float get_time(BlendState5C224 *blend)
    {
        return blend->time;
    }
};

struct BlendModel5C224 {
    char pad_000[0x10];
    BlendState5C224 blend;
};

struct BlendOwner5C224 {
    char pad_000[0x314];
    BlendModel5C224 *model;
};

extern "C" {

bool func_08A5C224(BlendOwner5C224 *self)
{
    return BlendState5C224::is_active(&self->model->blend);
}

float func_08A5C23C(BlendOwner5C224 *self)
{
    return BlendState5C224::get_time(&self->model->blend);
}

}
