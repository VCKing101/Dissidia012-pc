/* Advances the phase (field 1) by rate * field 0 and wraps it once past 1.0. */
extern float D_089D5EAC;
void func_08AC5588(float *p)
{
    float phase = p[1] + D_089D5EAC * p[0];
    float unity = 1.0f;
    p[1] = phase;
    if (phase > unity) {
        p[1] = phase - unity;
    }
}
