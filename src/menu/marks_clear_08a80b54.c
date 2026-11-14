typedef struct ClearMark {
    char pad_00[0x30];
    int field_30;
    int field_34;
    int field_38;
} ClearMark;

typedef struct ClearMarks {
    char pad_000[0x408];
    ClearMark *field_408[1];
} ClearMarks;

typedef struct ClearOwner {
    char pad_00[0x28];
    ClearMarks *field_28;
} ClearOwner;

typedef struct ClearLink {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
} ClearLink;

extern ClearLink *func_08A85EC0(ClearOwner *self, int id);

void func_08A80B54(ClearOwner *self, int id)
{
    ClearLink *links = func_08A85EC0(self, id);
    int i;
    for (i = 0; i < 0x14; i++) {
        if (links[i].field_00 == id) {
            ClearMark *mark = self->field_28->field_408[links[i].field_04];
            mark->field_34 = 0;
            mark->field_30 = 0;
            mark->field_38 = 0;
        }
    }
}
