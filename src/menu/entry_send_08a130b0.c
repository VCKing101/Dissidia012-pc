typedef struct SendEntry {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    char pad_10[0x6B18 - 0x10];
} SendEntry;

typedef struct SendTable {
    char pad_000[0x274];
    SendEntry field_274[1];
} SendTable;

typedef struct SendLink {
    char pad_000[0xECC];
    SendTable *field_ECC;
} SendLink;

typedef struct SendOwner {
    char pad_000[0xF60];
    SendLink *field_F60;
} SendOwner;

extern int D_08A23FF0;
extern void func_08A1F610(int queue, int *payload);

int func_08A130B0(SendOwner *self, int index)
{
    SendEntry *entries = self->field_F60->field_ECC->field_274;
    int *payload;
    int result = 0;
    if (entries[index].field_00 == 2) {
        payload = &entries[index].field_0C;
    } else {
        payload = 0;
    }
    if (payload != 0) {
        func_08A1F610(D_08A23FF0, payload);
        result = 1;
    }
    return result;
}
