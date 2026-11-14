struct ForwardOwner;

/* Child object reached through +0x208; its third virtual takes the owner and three values. */
class ForwardChild {
public:
    virtual int v0(ForwardOwner *owner, int a, int b, int c);
    virtual int v1(ForwardOwner *owner, int a, int b, int c);
    virtual int v2(ForwardOwner *owner, int a, int b, int c);
};

struct ForwardOwner {
    char pad_000[0x208];
    ForwardChild *field_208;
};

extern "C" {
int func_08A6FC24(ForwardOwner *self, int a, int b, int c)
{
    if (self->field_208 != 0) {
        return self->field_208->v2(self, a, b, c);
    }
    return 0;
}
}
