typedef struct Res { int field_00; int field_04; int handle; } Res;
typedef struct ResOwner { char pad_00[0x88]; Res *res; } ResOwner;
extern void func_089B04D8(int handle);
void func_0886A908(ResOwner *self)
{
    if (self->res == 0) {
        return;
    }
    func_089B04D8(self->res->handle);
    self->res = 0;
}
