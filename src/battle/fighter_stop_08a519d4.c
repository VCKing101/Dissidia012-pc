typedef struct Vec3519D4 {
    float x;
    float y;
    float z;
} Vec3519D4;

typedef struct Fighter519D4 {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x20];
    Vec3519D4 *velocity;
} Fighter519D4;

void func_08A519D4(void *self, Fighter519D4 *fighter)
{
    Vec3519D4 *velocity = fighter->velocity;
    velocity->x = 0.0f;
    velocity->y = 0.0f;
    velocity->z = 0.0f;
    fighter->flags &= ~0x100;
}
