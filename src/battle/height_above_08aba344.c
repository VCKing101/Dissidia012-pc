typedef struct BodyPose {
    char pad_00[0xC];
    float height;
} BodyPose;

typedef struct BodyUnit {
    unsigned char active;
    char pad_01[0x47];
    BodyPose *pose;
} BodyUnit;

typedef struct BodyRef {
    int pad_00;
    BodyUnit *unit;
} BodyRef;

/* The unit's height plus 100, 0 for an inactive unit. */
float func_08ABA344(BodyRef *self)
{
    BodyUnit *unit = self->unit;
    if (unit->active == 0) {
        return 0.0f;
    }
    return unit->pose->height + 100.0f;
}
