/* Random float between lo and hi (a member: self is unused) from the current context's seed
   (LCG 0x343FD / 0x269EC3, 10001 steps). */
int func_0881BAA8(void);
int *func_0881BA98(int context);

float func_08820CBC(void *self, float lo, float hi)
{
    int *seed = func_0881BA98(func_0881BAA8());
    int next = *seed * 0x343FD + 0x269EC3;
    *seed = next;
    return (hi - lo) * ((float)((next >> 16 & 0x7FFF) % 10001) / 10000.0f) + lo;
}
