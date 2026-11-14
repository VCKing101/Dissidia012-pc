/* Goes on when the save data is unchanged and not locked, otherwise asks first. */
typedef struct CheckData {
    char pad_00[0x10];
    int field_10;
    char field_14[0x6B0C];
} CheckData;

typedef struct CheckOwner {
    char pad_00[0x5C];
    CheckData *field_5C;
} CheckOwner;

typedef struct CheckFlags {
    char pad_00[0x13];
    unsigned char field_13;
} CheckFlags;

extern CheckFlags D_09B6DCE8;
extern int func_08935490(void *data, int checksum, int size);
extern void func_08A4EA6C(CheckOwner *self);
extern void func_08A4E2E4(CheckOwner *self, int text, void *onYes, void *onNo, void *onCancel,
                          int sound, int a, int b, int c);
extern void func_08A4D6B8(void);
extern void func_08A4D694(void);

void func_08A4E9B4(CheckOwner *self)
{
    if (func_08935490(self->field_5C->field_14, self->field_5C->field_10, 0x6B0C) == 0) {
        if (D_09B6DCE8.field_13 == 0) {
            func_08A4EA6C(self);
            return;
        }
    }
    func_08A4E2E4(self, 0, func_08A4D6B8, func_08A4D694, func_08A4D694, 0x1F, -1, -1, -1);
}
