/* Clears the motion state bits and counters and marks the unit dirty. */
typedef struct Vec4_08814C94 {
    float x;
    float y;
    float z;
    float w;
} Vec4_08814C94;

typedef struct MotionBits08814C94 {
    unsigned int active : 1;
    unsigned int moving : 1;
    unsigned int turning : 1;
    unsigned int landed : 1;
    unsigned int phase : 4;
} MotionBits08814C94;

typedef struct DirtyBits08814C94 {
    unsigned int dirty : 1;
} DirtyBits08814C94;

typedef struct Unit08814C94 {
    char pad_000[0x378];
    DirtyBits08814C94 dirty;
    char pad_37C[0x1D4];
    Vec4_08814C94 velocity;
    MotionBits08814C94 motion;
    int field_564;
    int field_568;
    char pad_56C[0x88];
    int field_5F4;
    int field_5F8;
} Unit08814C94;

void func_089A3890(Vec4_08814C94 *v);

void func_08814C94(Unit08814C94 *unit)
{
    unit->motion.active = 0;
    unit->motion.moving = 0;
    unit->motion.turning = 0;
    unit->motion.landed = 0;
    unit->motion.phase = 0;
    unit->field_564 = 0;
    unit->field_568 = 0;
    func_089A3890(&unit->velocity);
    unit->field_5F8 = 0;
    unit->dirty.dirty = 1;
    unit->field_5F4 = 0;
}
