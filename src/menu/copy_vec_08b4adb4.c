/* Copies the three floats at +0x64 of **src into *dst. */
struct Vec { float x, y, z; };
struct Src { char pad[0x64]; struct Vec v; };
void func_08B4ADB4(struct Vec *dst, struct Src **src)
{
    *dst = (*src)->v;
}
