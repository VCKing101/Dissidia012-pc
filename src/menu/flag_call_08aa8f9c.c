typedef struct CallCtx {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
} CallCtx;

typedef struct CallOwner {
    char pad_0000[0x7CFC];
    CallCtx *field_7CFC;
} CallOwner;

extern int func_08AAEF00(CallOwner *self);

int func_08AA8F9C(CallOwner *self)
{
    int result = 0;
    if ((self->field_7CFC->field_7CC0 & 0x200) == 0) {
        result = func_08AAEF00(self);
    }
    return result;
}
