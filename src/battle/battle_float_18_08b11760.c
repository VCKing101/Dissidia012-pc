// Returns the float at offset 0x18 of the object pointed to by global D_089D69E0.
struct B089D69E0 { char pad[0x18]; float v; };
extern struct B089D69E0 *D_089D69E0;
float func_08B11760(void)
{
    return D_089D69E0->v;
}
