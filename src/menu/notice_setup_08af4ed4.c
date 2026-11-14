/* Fills in the notice defaults (or uses the page's own notice) and shows it. */
typedef struct NoticeInfo {
    void *field_00;
    void *field_04;
    void *field_08;
    char pad_0C[0x8];
    void *field_14;
    void *field_18;
} NoticeInfo;

typedef struct NoticeState {
    char pad_0000[0x89C];
    void *field_89C;
    char pad_08A0[0x2C40 - 0x8A0];
    NoticeInfo field_2C40;
} NoticeState;

typedef struct NoticeOwner {
    char pad_000[0x140];
    NoticeState *field_140;
} NoticeOwner;

extern char D_08B0067E[];
extern char D_08B00926[];
extern char D_08B003C6[];
extern char D_08B00996[];
extern void func_088D3CF0(void *a, void *b, void *table);
extern void func_088D3F7C(void *window, void *text, void *table, int zero);

void func_08AF4ED4(NoticeOwner *self, NoticeInfo *info)
{
    if (info == 0) {
        info = &self->field_140->field_2C40;
        func_088D3CF0(info->field_00, info->field_04, D_08B0067E);
    }
    if (info->field_14 == 0) {
        info->field_14 = D_08B00926;
    }
    if (info->field_18 == 0) {
        info->field_18 = D_08B003C6;
    }
    func_088D3F7C(self->field_140->field_89C, info->field_08, D_08B00996, 0);
}
