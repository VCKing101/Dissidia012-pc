/* Ratio of the first measure to half the square of the second. */
float func_0884E504(void *obj);
float func_0884E4F4(void *obj);

float func_0884E40C(void *obj)
{
    float a = func_0884E504(obj);
    float b = func_0884E4F4(obj);
    float c = func_0884E4F4(obj);
    return a / (b * c * 0.5f);
}
