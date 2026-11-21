/* Converts the curve's two angles from degrees to radians and passes them on; 0 when inactive or no curve. */
typedef struct Cv38 { char pad[0x10]; float x; float y; } Cv38;
extern float func_08AC3DE0(float x, float y);
extern float D_08ABC5F0;
float func_08ABC3BC(unsigned char *self, Cv38 *c)
{
    if (*self == 0) {
        return 0;
    } else if (c == 0) {
        return 0;
    } else {
        return func_08AC3DE0(c->x * D_08ABC5F0 / 180.0f, c->y * D_08ABC5F0 / 180.0f);
    }
}
