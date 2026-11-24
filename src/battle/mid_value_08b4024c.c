/* Stores the mean of two floats of entry `index` (0x94 bytes each, from +0x44 of the table at 0x089D4798) at +4. */
struct Entry { char pad[0x94]; };
struct Out { float x; float y; };
extern struct Entry *D_089D4798;
void func_08B4024C(struct Out *out, int index)
{
    float *f = (float *)((char *)&D_089D4798[index] + 0x44);
    out->y = (f[0] + f[1]) * 0.5f;
}
