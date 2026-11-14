/* Loads every sound of the bank's id list that is set (id >= 0). */
typedef struct SoundBankHead {
    char pad_0[0xC];
    short field_0C;
} SoundBankHead;

typedef struct SoundBank {
    SoundBankHead *field_00;
    char pad_04[0x8];
    short *field_0C;
} SoundBank;

typedef struct SoundRequest {
    int field_00;
} SoundRequest;

typedef struct SoundFlags {
    int field_00;
} SoundFlags;

extern "C" {
void func_088C1C0C(SoundRequest *request);
void func_088C1CD4(SoundRequest *request, char *work, int id, int channel);

void func_08A48AA4(void *self, SoundBank *bank, SoundFlags flags)
{
    SoundRequest request;
    char work[0x17C];
    int count = bank->field_00->field_0C;
    for (int i = 0; i < count; i++) {
        int id = bank->field_0C[i * 2];
        if (id >= 0) {
            func_088C1C0C(&request);
            func_088C1CD4(&request, work, id, -1);
        }
    }
}
}
