/* Integer between from and to at position pos of len, clamped to the ends; 0 for an empty length. */
int func_08AC3E5C(float pos, float len, int from, int to)
{
    float t;
    if (len == 0.0f) {
        return 0;
    }
    t = pos / len;
    if (t < 0.0f) {
        t = 0.0f;
    } else if (t > 1.0f) {
        t = 1.0f;
    }
    return (int)((float)(to - from) * t) + from;
}
