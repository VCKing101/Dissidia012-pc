typedef struct OpenCursor {
    char pad_00[0x10];
    short field_10;
} OpenCursor;

typedef struct OpenLimits {
    char pad_00[0x18];
    short field_18;
} OpenLimits;

typedef struct OpenEntry {
    char data[0x20];
} OpenEntry;

typedef struct OpenState {
    char pad_00[0x40];
    OpenLimits *field_40;
    char pad_44[0x10];
    OpenEntry *field_54;
    char pad_58[0x10];
    OpenCursor *field_68;
} OpenState;

typedef struct OpenOwner {
    char pad_000[0x14C];
    OpenState *field_14C;
} OpenOwner;

extern int func_089F0340(OpenOwner *self, OpenEntry *entry, int kind);

int func_089F04B4(OpenOwner *self)
{
    int result = 0;
    int index = 0;
    if (self->field_14C->field_68->field_10 >= 0) {
        index = self->field_14C->field_68->field_10;
    }
    if (index < self->field_14C->field_40->field_18) {
        result = func_089F0340(self, &self->field_14C->field_54[index], 9);
    }
    return result;
}
