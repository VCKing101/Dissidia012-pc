extern float func_08AC3D20(float a, float b);
extern float func_089A4774(float x, float y);

/* Cube root of the combined cubes of a and b. */
float func_08AC3DA4(float a, float b)
{
    return func_089A4774(func_08AC3D20(a * a * a, b * b * b), 0.33333334f);
}
