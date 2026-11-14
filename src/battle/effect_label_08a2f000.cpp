struct LabelInfo {
    char pad_00[0x14];
    unsigned int id;
    unsigned char visible;
};

struct LabelOwner {
    char pad_000[0x130];
    char field_130[0x254];
    unsigned int flags;
    char pad_388[0x20];
    LabelInfo *info;

    static bool isHidden(LabelOwner *owner) { return (owner->flags & 0x200) != 0; }
};

extern "C" {

extern void func_08935828(void *dst, int value, int size);
extern void func_0882FA10(float x, float y, void *sub, int id, int arg);
extern void func_08837A5C(LabelOwner *self);

void func_08A2F000(int unused, LabelOwner *self, unsigned short id)
{
    LabelInfo *info = self->info;
    func_08935828(info, 0, 0x20);
    info->visible = !LabelOwner::isHidden(self);
    info->id = id;
    func_0882FA10(0.0f, 0.0f, self->field_130, 0x4E3E, 0);
    self->flags |= 0x200000;
    func_08837A5C(self);
}

}
