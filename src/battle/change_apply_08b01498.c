typedef struct ChangeSource {
    char pad_00[0x4];
    int amount;
    short from;
    short to;
} ChangeSource;

typedef struct ChangeTarget {
    char pad_00[0x1];
    signed char kind;
    char pad_02[0x2];
    short value;
} ChangeTarget;

extern void func_088CBD24(ChangeTarget *target, int from, int to);

void func_08B01498(void *self, ChangeTarget *target, ChangeSource *source)
{
    target->kind = (signed char)source->to;
    target->value = (short)source->amount;
    if (source->from != source->to) {
        func_088CBD24(target, source->from, source->to);
    }
}
