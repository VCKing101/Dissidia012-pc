typedef struct Motion {
    int field_00;
    int field_04;
    float time;
    unsigned char field_0C;
    unsigned char field_0D;
    unsigned char field_0E;
    unsigned char field_0F;
} Motion;

void func_0891191C(Motion *motion)
{
    motion->field_00 = 0;
    motion->field_04 = 0;
    motion->time = 0.0f;
    motion->field_0C = 0;
    motion->field_0D = 0;
    motion->field_0E = 0;
    motion->field_0F = 0;
}
