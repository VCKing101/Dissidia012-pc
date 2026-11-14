/* Ratio of the first measure to half the square of the second. */
float func_0884E4C4(void *obj);
float func_0884E4B4(void *obj);

float func_0884E3AC(void *obj)
{
    float a = func_0884E4C4(obj);
    float b = func_0884E4B4(obj);
    float c = func_0884E4B4(obj);
    return a / (b * c * 0.5f);
}
