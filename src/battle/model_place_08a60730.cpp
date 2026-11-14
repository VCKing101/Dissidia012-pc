struct Vec360730 {
    float x;
    float y;
    float z;
};

struct Placement60730 {
    Vec360730 position;

    static void set_position(Placement60730 *placement, const Vec360730 *position)
    {
        placement->position.x = position->x;
        placement->position.y = position->y;
        placement->position.z = position->z;
    }
};

struct Model60730 {
    char pad_000[0x48];
    Placement60730 placement;
};

struct Fighter60730 {
    char pad_000[0x314];
    Model60730 *model;
};

extern "C" {

void func_08A60730(void *self, Fighter60730 *fighter, const Vec360730 *position)
{
    Placement60730::set_position(&fighter->model->placement, position);
}

}
