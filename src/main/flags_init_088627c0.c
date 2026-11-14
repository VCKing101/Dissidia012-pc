typedef struct Flags { unsigned char a; unsigned char b; unsigned char c; char pad_03; int d; int e; int f; } Flags;
Flags *func_088627C0(Flags *self)
{
    unsigned char one = 1;

    self->a = 0;
    self->b = 0;
    self->c = one;
    self->d = 0;
    self->e = 0;
    self->f = 0;
    return self;
}
