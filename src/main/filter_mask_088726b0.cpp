struct Vec8 {
    float v[8];
};

struct MaskNode {
    unsigned long long mask;
    char pad_08[0x18];
    Vec8 field_20;
};

struct MaskFilter {
    char pad_00[0x20];
    Vec8 field_20;
    MaskNode *node;
    int field_44;
    int field_48;
    void *extra;
};

extern "C" {

extern char D_089C9738[];
unsigned long long func_0887263C(MaskFilter *self, MaskNode *node, unsigned long long mask, void *table);
void func_089A3868(Vec8 *dst, Vec8 *src);

unsigned long long func_088726B0(MaskFilter *self)
{
    unsigned long long r = func_0887263C(self, self->node, self->node->mask, D_089C9738);
    if (self->extra != 0) {
        r |= func_0887263C(self, self->node, self->node->mask, self->extra);
    }
    Vec8 *src = &self->node->field_20;
    func_089A3868(&self->field_20, src);
    return r;
}

}
