typedef struct StreamSet {
    unsigned char active;
    char pad_01[0xB];
    char field_0C[0x4];
} StreamSet;

extern int func_0887AFB0(void *stream);

unsigned char func_08B04F80(StreamSet *self)
{
    int result = 0;
    if (self->active == 1 && func_0887AFB0(self->field_0C) == 1) {
        result = 1;
    }
    return result;
}
