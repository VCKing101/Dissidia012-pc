/* Clears the masks of the definition's two records from the flags. */
struct Rec088C1A5C {
    char pad_00[0x10];
    float base;
    float value;
    char pad_18[8];
    unsigned int mask;
};
struct Def088C1A5C {
    char pad_00[0x30];
    short ids[2];
};
struct Ref088C1A5C {
    Rec088C1A5C *rec;
    Def088C1A5C *def;
};
extern "C" {
Rec088C1A5C *func_088C7E98(int id);
unsigned int func_088C1A5C(Ref088C1A5C *ref, unsigned int flags)
{
    if (ref->def != 0) {
        for (int i = 0; i < 2; i++) {
            short *ids = ref->def->ids;
            Rec088C1A5C *rec = func_088C7E98(ids[i]);
            if (rec != 0) {
                flags &= ~rec->mask;
            }
        }
    }
    return flags;
}
}
