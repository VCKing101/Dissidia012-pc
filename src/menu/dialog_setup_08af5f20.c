/* Sets up the yes/no dialog item (with its question shown when `ask` is set). */
typedef struct DialogIcons {
    char pad_00[0x60];
    void *field_60;
} DialogIcons;

typedef struct DialogItem {
    char pad_00[0x4];
    int field_04;
    int field_08;
    char pad_0C[0x4];
    DialogIcons *field_10;
    void *field_14;
    char pad_18[0x4C - 0x18];
    void *field_4C;
    void *field_50;
    void *field_54;
    int field_58;
    void *field_5C;
    void *field_60;
} DialogItem;

typedef struct DialogState {
    char pad_000[0x89C];
    void *field_89C;
} DialogState;

typedef struct DialogOwner {
    char pad_000[0x140];
    DialogState *field_140;
} DialogOwner;

extern char D_08B00CD2[];
extern char D_08B00D26[];
extern char D_08B00D4E[];
extern char D_08B00D14[];
extern char D_08B016AC[];
extern char D_08B016B8[];
extern void func_088D3F7C(void *window, void *text, void *table, int arg);
extern void func_08AF5C34(DialogOwner *self, DialogItem *item, int kind);
extern void func_088D9F5C(void *icon, int frame, int one);

void func_08AF5F20(DialogOwner *self, DialogItem *item, unsigned char ask)
{
    item->field_4C = D_08B00CD2;
    item->field_50 = D_08B00D26;
    item->field_54 = D_08B00D4E;
    item->field_58 = 0;
    item->field_04 = 0;
    if (ask != 0) {
        func_088D3F7C(self->field_140->field_89C, item->field_14, D_08B00D14, item->field_58);
        item->field_08 = 5;
    } else {
        item->field_08 = 4;
    }
    item->field_5C = D_08B016AC;
    item->field_60 = D_08B016B8;
    func_08AF5C34(self, item, 6);
    if (ask != 0) {
        func_088D9F5C(item->field_10->field_60, 4, 1);
    }
}
