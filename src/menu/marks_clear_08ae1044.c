typedef struct ClearMark {
    char pad_00[0x30];
    int field_30;
    int field_34;
    int field_38;
} ClearMark;

typedef struct ClearMarks {
    char pad_000[0x400];
    ClearMark *field_400[1];
} ClearMarks;

typedef struct ClearOwner {
    char pad_000[0x850];
    ClearMarks *field_850;
} ClearOwner;

typedef struct ClearLink {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
} ClearLink;

extern ClearLink *func_08AE4714(ClearOwner *self, int id);

void func_08AE1044(ClearOwner *self, int id)
{
    ClearLink *links = func_08AE4714(self, id);
    int i;
    for (i = 0; i < 0x11; i++) {
        if (links[i].field_00 == id) {
            ClearMark *mark = self->field_850->field_400[links[i].field_04];
            mark->field_34 = 0;
            mark->field_30 = 0;
            mark->field_38 = 0;
        }
    }
}
