typedef struct IndexInner {
    char pad_00[8];
    int field_08;
    char pad_0C[8];
    short field_14;
} IndexInner;

typedef struct IndexOwner {
    char pad_00[4];
    IndexInner *field_04;
} IndexOwner;

int func_08A972E0(IndexOwner *self, int key, int *out)
{
    int i = 0;
    IndexInner *inner = self->field_04;
    int count = inner->field_08;
    if (i < count) {
        do {
            if (inner->field_14 == key) {
                *out = i;
                return 1;
            }
            i++;
            inner = (IndexInner *)((char *)inner + 2);
        } while (i < count);
    }
    return 0;
}
