/* 1 when the point lies inside the box in x and z. */
typedef struct Vec3_0885BE40 {
    float x;
    float y;
    float z;
} Vec3_0885BE40;

typedef struct Box0885BE40 {
    Vec3_0885BE40 min;
    Vec3_0885BE40 max;
} Box0885BE40;

int func_0885BE40(Box0885BE40 *box, Vec3_0885BE40 *p)
{
    if (p->x < box->min.x || p->x > box->max.x) {
        return 0;
    }
    if (p->z < box->min.z || p->z > box->max.z) {
        return 0;
    }
    return 1;
}
