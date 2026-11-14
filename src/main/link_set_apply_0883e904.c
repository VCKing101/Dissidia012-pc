typedef struct LinkEntry {
    char data[0x1C];
} LinkEntry;

typedef struct LinkSet {
    int field_00;
    short count;
    short field_06;
    int field_08;
    LinkEntry entries[1];
} LinkSet;

typedef struct Mat4x1 {
    float v[4];
} Mat4x1;

extern LinkEntry *func_0883EA64(LinkSet *set, LinkEntry *key);
extern void func_08812EF0(LinkEntry *found, LinkEntry *key, Mat4x1 *out);
extern void func_0883E3BC(LinkSet *set, LinkSet *other, Mat4x1 *acc, Mat4x1 *v);
extern void func_089A3978(Mat4x1 *out, Mat4x1 *in);
extern void func_0883E2F4(LinkSet *set, Mat4x1 *v);
extern void func_089A38E0(Mat4x1 *out, Mat4x1 *a, Mat4x1 *b);

void func_0883E904(LinkSet *set, LinkSet *other, Mat4x1 *acc)
{
    Mat4x1 delta;
    Mat4x1 inv;
    short count = other->count;
    int i;

    for (i = 0; i < count; i++) {
        LinkEntry *found = func_0883EA64(set, &other->entries[i]);
        if (found != 0) {
            func_08812EF0(found, &other->entries[i], &delta);
            func_0883E3BC(set, other, acc, &delta);
            func_089A3978(&inv, &delta);
            func_0883E2F4(set, &inv);
            func_089A38E0(acc, acc, &delta);
        }
    }
}
