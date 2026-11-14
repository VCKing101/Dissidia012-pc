typedef struct Mark59134 {
    float field_00;
    unsigned char field_04;
} Mark59134;

typedef struct Fighter59134 {
    char pad_000[0x3A8];
    Mark59134 *mark;
} Fighter59134;

void func_08A59134(void *self, Fighter59134 *fighter)
{
    Mark59134 *mark = fighter->mark;
    mark->field_00 = 0.0f;
    mark->field_04 = 1;
}
