/* turn a phase and a width into a pair of ratios (sine and cosine over 1 + 2 * width). */
extern float func_08867ED0(float x);
extern float func_08867EEC(float x);
void func_089912C8(float *a, float *b)
{
    float ang = (*a * 0.125f + 0.125f) * 6.2832f;
    float w = *b * 2.0f;
    float s = func_08867ED0(ang);
    w = w + 1.0f;
    s = s / w;
    float c = func_08867EEC(ang) / w;
    *a = s;
    *b = c;
}
