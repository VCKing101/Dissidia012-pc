/* Computes a damped step toward a target and writes both the step and the new value. */
void func_08B3B99C(float a, float b, float e, float d, float c, float *out_a, float *out_step) {
    float t = (-((a - b) * d)) - (c * e) + e;
    *out_step = t;
    *out_a = a + t;
}
