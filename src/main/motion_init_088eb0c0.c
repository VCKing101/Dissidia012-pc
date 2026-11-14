/* Initialises a motion record (scale 1) and returns it. */
typedef struct MotionRecord {
    float x;
    float y;
    float z;
    float w;
    float scale;
    float time;
    int field_18;
    int field_1C;
    unsigned char active;
    unsigned char done;
} MotionRecord;

MotionRecord *func_088EB0C0(MotionRecord *m)
{
    m->x = 0.0f;
    m->y = 0.0f;
    m->z = 0.0f;
    m->w = 0.0f;
    m->scale = 1.0f;
    m->time = 0.0f;
    m->field_18 = 0;
    m->field_1C = 0;
    m->active = 0;
    m->done = 0;
    return m;
}
