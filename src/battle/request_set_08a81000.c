typedef struct RequestSet81000 {
    unsigned char active;
    char pad_001[0x5F];
    unsigned char field_60;
    unsigned char field_61;
    char pad_062[0x2];
    int field_64;
    unsigned char field_68;
} RequestSet81000;

int func_08A81000(RequestSet81000 *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_60 = 1;
    return 0;
}

int func_08A81028(RequestSet81000 *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_61 = 1;
    return 0;
}

int func_08A81050(RequestSet81000 *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_64 = 10;
    return 0;
}

int func_08A81078(RequestSet81000 *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_68 = 1;
    return 0;
}
