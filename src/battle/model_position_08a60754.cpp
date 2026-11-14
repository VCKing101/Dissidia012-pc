struct Vec460754 {
    float x;
    float y;
    float z;
    float w;
};

struct Placement60754 {
    float x;
    float y;
    float z;
};

struct Model60754 {
    char pad_000[0x48];
    Placement60754 placement;

    static Placement60754 *placement_of(Model60754 *model)
    {
        return &model->placement;
    }
};

struct Fighter60754 {
    char pad_000[0x314];
    Model60754 *model;
};

extern "C" {

Vec460754 func_08A60754(void *self, Fighter60754 *fighter)
{
    Placement60754 *placement = Model60754::placement_of(fighter->model);
    Vec460754 result;
    result.x = placement->x;
    result.y = placement->y;
    result.z = placement->z;
    result.w = 1.0f;
    return result;
}

}
