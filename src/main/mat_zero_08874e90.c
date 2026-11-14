/* Zeroes a 17-float block and returns it. */
typedef struct Block08874E90 {
    float v[17];
} Block08874E90;

Block08874E90 *func_08874E90(Block08874E90 *b)
{
    b->v[0] = 0.0f;
    b->v[1] = 0.0f;
    b->v[2] = 0.0f;
    b->v[3] = 0.0f;
    b->v[4] = 0.0f;
    b->v[5] = 0.0f;
    b->v[6] = 0.0f;
    b->v[7] = 0.0f;
    b->v[8] = 0.0f;
    b->v[9] = 0.0f;
    b->v[10] = 0.0f;
    b->v[11] = 0.0f;
    b->v[12] = 0.0f;
    b->v[13] = 0.0f;
    b->v[14] = 0.0f;
    b->v[15] = 0.0f;
    b->v[16] = 0.0f;
    return b;
}
