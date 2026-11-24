/* Sets the four floats at 0x08B51860 to 0, 28, 0, 1. */
struct V { float x, y, z, w; };
extern struct V D_08B51860;
void func_08B516EC(void)
{
    D_08B51860.x = 0.0f;
    D_08B51860.y = 28.0f;
    D_08B51860.z = 0.0f;
    D_08B51860.w = 1.0f;
}
