/* Opens a bone rig: checks every argument, binds the parts to their models
 * and keeps the names and tables it reads later. */
struct BoneRig {
    unsigned char open;
    unsigned char pad_01[7];
    unsigned char parts[0x40];
    unsigned int side;
    void *source;
    void *bones;
    int name;
    void *offsets;
    void *scales;
    void *colors;
};

extern void func_08A12CBC(struct BoneRig *rig);
extern int func_089F7D74(void *parts, void *models, void *names);

int func_08A12D6C(struct BoneRig *rig, unsigned int side, void *source, void *models, void *names, void *bones,
                  int name, void *offsets, void *scales, void *colors)
{
    func_08A12CBC(rig);
    if (side >= 2 || source == 0 || models == 0 || names == 0 || bones == 0 || name < 0 || offsets == 0
        || scales == 0 || colors == 0) {
        return -1;
    }
    if (func_089F7D74(rig->parts, models, names) != 0) {
        return -2;
    }
    rig->source = source;
    rig->side = side;
    rig->bones = bones;
    rig->name = name;
    rig->offsets = offsets;
    rig->scales = scales;
    rig->colors = colors;
    rig->open = 1;
    return 0;
}
