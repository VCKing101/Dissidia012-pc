typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct PosInfo57400 {
    Vec3f pos;
    char pad_0C[0x4];
    unsigned char active;
} PosInfo57400;

typedef struct Fighter57400 {
    char pad_000[0x3A8];
    PosInfo57400 *info;
} Fighter57400;

extern void func_08A573CC(Fighter57400 *fighter);

void func_08A57400(Fighter57400 *self, Vec3f *pos)
{
    PosInfo57400 *info;
    func_08A573CC(self);
    info = self->info;
    info->pos.x = pos->x;
    info->pos.y = pos->y;
    info->pos.z = pos->z;
    info->active = 1;
}
