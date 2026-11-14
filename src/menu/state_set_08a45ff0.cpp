extern "C" {
extern int D_089D69E8;
extern int func_088857DC(int o);
}
struct Win45FF0 { char pad[0x418]; struct { char pad[0x3E8]; int state; } *owner; };
extern "C" void func_08A45FF0(Win45FF0 *self)
{
    if (func_088857DC(D_089D69E8) == 0) {
        return;
    }
    self->owner->state = 3;
}
