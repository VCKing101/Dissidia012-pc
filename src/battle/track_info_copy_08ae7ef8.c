typedef struct TrackSource {
    int field_00;
    int field_04;
    float field_08;
    int pad_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    unsigned int flags;
} TrackSource;

typedef struct TrackInfo {
    int field_00;
    int field_04;
    float field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    unsigned char looping;
    unsigned char mirrored;
} TrackInfo;

void func_08AE7EF8(TrackSource *src, TrackInfo *out)
{
    out->field_00 = src->field_00;
    out->field_04 = src->field_04;
    out->field_08 = src->field_08;
    out->looping = (src->flags & 4) != 0;
    out->mirrored = (src->flags & 8) != 0;
    out->field_0C = src->field_10;
    out->field_10 = src->field_14;
    out->field_14 = src->field_18;
    out->field_18 = src->field_1C;
    out->field_1C = src->field_20;
    out->field_20 = src->field_24;
}
