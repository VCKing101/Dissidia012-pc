/* Returns a float chosen by mode +0x100: 0 -> field of a global, 1 -> global, 2 -> own field at +0x104, else 0. */
struct Obj { char pad[0x100]; int mode; float v; };
struct G { char pad[0x18]; float f; };
extern struct G *D_089D69E0;
extern float D_089D5EAC;
float func_08B3B4FC(struct Obj *self)
{
    switch (self->mode) {
    case 0:
        return D_089D69E0->f;
    case 1:
        return D_089D5EAC;
    case 2:
        return self->v;
    }
    return 0.0f;
}
