typedef struct MarkOwner {
    char pad_00[0x28];
    unsigned int flags;
} MarkOwner;

typedef struct MarkSource {
    char pad_00[0x30];
    char list_30[1];
} MarkSource;

extern "C" {

extern int func_08AE96D4(void *list);

bool func_08AE9578(MarkOwner *self, int a, int b, MarkSource *source)
{
    if (func_08AE96D4(source->list_30) == 1) {
        self->flags |= 4;
        return true;
    }
    return false;
}

}
