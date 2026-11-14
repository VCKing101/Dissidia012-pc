/* Copies the state's current values and its slot block into `out`. */
struct Block08B2E610 {
    int words[0x45];
};

struct Saved08B2E610 {
    char pad_00[8];
    int field_08;
    int field_0C;
    int field_10;
    char pad_14[4];
    struct Block08B2E610 field_18;
};

struct State08B2E610 {
    char pad_000[0x560];
    struct Block08B2E610 field_560;
    char pad_674[0x678 - 0x674];
    int field_678;
    char pad_67C[0x824 - 0x67C];
    int field_824;
    char pad_828[4];
    int field_82C;
};

struct Owner08B2E610 {
    char pad_000[0x844];
    struct State08B2E610 *field_844;
};

void func_08B2E610(struct Owner08B2E610 *self, struct Saved08B2E610 *out)
{
    out->field_08 = self->field_844->field_824;
    out->field_0C = self->field_844->field_678;
    out->field_10 = self->field_844->field_82C;
    out->field_18 = self->field_844->field_560;
}
