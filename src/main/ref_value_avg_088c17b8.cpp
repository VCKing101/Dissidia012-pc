/* Average positive value of the definition's two records; the record's base value when that is 0. */
struct Rec088C17B8 {
    char pad_00[0x10];
    float base;
    float value;
    char pad_18[8];
    unsigned int mask;
};
struct Def088C17B8 {
    char pad_00[0x30];
    short ids[2];
};
struct Ref088C17B8 {
    Rec088C17B8 *rec;
    Def088C17B8 *def;
};
extern "C" {
Rec088C17B8 *func_088C7E98(int id);
float func_088C17B8(Ref088C17B8 *ref)
{
    float sum = 0.0f;
    if (ref->def != 0) {
        int count = 0;
        for (int i = 0; i < 2; i++) {
            short *ids = ref->def->ids;
            Rec088C17B8 *rec = func_088C7E98(ids[i]);
            if (rec != 0 && 0.0f < rec->value) {
                sum += rec->value;
                count++;
            }
        }
        if (count > 0) {
            sum /= count;
        }
    }
    if (sum == 0.0f && ref->rec != 0) {
        sum = ref->rec->base;
    }
    return sum;
}
}
