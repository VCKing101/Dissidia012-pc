/* Four 64-bit masks; each test takes its mask in the aligned a2:a3 pair. */
typedef struct MaskSet {
    unsigned long long m0;
    unsigned long long m1;
    unsigned long long m2;
    unsigned long long m3;
} MaskSet;

int func_08871CFC(MaskSet *self, int unused, unsigned long long m)
{
    return (self->m0 & m) != 0;
}

int func_08871D18(MaskSet *self, int unused, unsigned long long m)
{
    return (self->m1 & m) != 0;
}

int func_08871D34(MaskSet *self, int unused, unsigned long long m)
{
    return (self->m2 & m) != 0;
}

int func_08871D50(MaskSet *self, int unused, unsigned long long m)
{
    return (self->m3 & m) != 0;
}
