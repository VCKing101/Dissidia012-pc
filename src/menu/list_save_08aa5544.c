/* Refreshes the list, then copies its 0x16DC-byte state out. */
typedef struct SaveListState {
    int field_00[0x16DC / 4];
} SaveListState;

typedef struct SaveListData {
    char pad_000[0x710];
    SaveListState field_710;
} SaveListData;

typedef struct SaveList {
    char pad_0000[0x1EE4];
    SaveListData *field_1EE4;
} SaveList;

extern void func_08AA54B8(SaveList *self);
extern void func_08AA5504(SaveList *self);
extern void func_08AA5524(SaveList *self);

void func_08AA5544(SaveList *self, SaveListState *out)
{
    func_08AA54B8(self);
    func_08AA5504(self);
    func_08AA5524(self);
    *out = self->field_1EE4->field_710;
}
