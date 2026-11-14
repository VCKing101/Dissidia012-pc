/* Sets five (start, target) pairs: starts from the global camera, targets from the arguments. */
typedef struct CameraSource {
    float field_00;
    float x;
    float y;
    float z;
    float w;
} CameraSource;

typedef struct MotionPairs {
    float x0, x1;
    float y0, y1;
    float z0, z1;
    float w0, w1;
    float t0, t1;
    unsigned char active;
} MotionPairs;

extern CameraSource *D_089D6040;

void func_088E09AC(MotionPairs *m, float x, float y, float z, float w, float t)
{
    CameraSource *src = D_089D6040;

    m->x0 = src->x;
    m->x1 = x;
    m->y0 = src->y;
    m->y1 = y;
    m->z0 = src->z;
    m->z1 = z;
    m->w0 = src->w;
    m->w1 = w;
    m->t1 = t;
    m->t0 = 0.0f;
    m->active = 1;
}
