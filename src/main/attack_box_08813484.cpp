/* The hit box of a throw-type attack (kinds 11-15); zeros otherwise. */
struct AttackDef08813484 {
    char pad_00[0x18];
    float x;
    float y;
    float z;
    float width;
    float height;
    short frames;
};

struct Attack08813484 {
    char pad_00[0x30];
    AttackDef08813484 *def;
    char pad_34;
    unsigned char kind;
};

extern "C" {

void func_08813484(Attack08813484 *self, float *x, float *y, float *z, float *width, float *height, int *frames)
{
    *x = 0.0f;
    *y = 0.0f;
    *z = 0.0f;
    *width = 0.0f;
    *height = 0.0f;
    switch (self->kind) {
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        if (self->def != 0) {
            *x = self->def->x;
            *y = self->def->y;
            *z = self->def->z;
            *width = self->def->width;
            *height = self->def->height;
            *frames = self->def->frames;
        }
        break;
    default:
        break;
    }
}

}
