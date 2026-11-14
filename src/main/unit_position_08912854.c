typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct UnitPose {
    char pad_000[0x1B4];
    float pos[3];               /* 0x1B4 */
    float dir[3];               /* 0x1C0 */
} UnitPose;

Vec4 func_08912854(UnitPose *unit)
{
    Vec4 v;
    v.x = unit->pos[0];
    v.y = unit->pos[1];
    v.z = unit->pos[2];
    v.w = 1.0f;
    return v;
}
