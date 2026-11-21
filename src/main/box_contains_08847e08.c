typedef struct Box08847E08 {
    unsigned char unk_00[0x18];
    unsigned int flags;                /* +0x18 bit 1: everything is inside */
    unsigned char unk_1C[8];
    float min_x;                       /* +0x24 */
    float max_x;                       /* +0x28 */
    float min_y;                       /* +0x2C */
    float max_y;                       /* +0x30 */
    float min_z;                       /* +0x34 */
    float max_z;                       /* +0x38 */
} Box08847E08;

typedef struct Body08847E08 {
    unsigned char unk_00[8];
    float radius;                      /* +0x08 */
} Body08847E08;

/* 1 when (x, y, z) is inside the box (y widened by the body's radius). */
unsigned char func_08847E08(float x, float y, float z, Box08847E08 *box, int unused, Body08847E08 *body)
{
    int inside;
    float y0;
    float y1;

    if ((unsigned char)((box->flags & 2) != 0)) {
        return 1;
    }
    y0 = box->min_y;
    y1 = box->max_y;
    y0 -= body->radius;
    y1 += body->radius;
    inside = 0;
    if (!(x < box->min_x) && x < box->max_x && !(y < y0) && y <= y1 && z <= box->max_z &&
        !(z <= box->min_z)) {
        inside = 1;
    }
    return inside;
}
