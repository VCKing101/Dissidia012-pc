/* Opens the list scroller on the current page's range and redraws the page. */
typedef struct ListRange {
    int field_00;
    int field_04;
} ListRange;

typedef struct ListScroller {
    unsigned int field_00;
} ListScroller;

typedef struct ListPage {
    char pad_0000[0x10F8];
    int field_10F8;
    char pad_10FC[0x1680 - 0x10FC];
    ListRange field_1680[(0x16B4 - 0x1680) / 8];
    char pad_16B0[0x4];
    int field_16B4;
    char pad_16B8[0x16DC - 0x16B8];
    ListScroller field_16DC;
} ListPage;

typedef struct ListOwner {
    char pad_0000[0x17B0];
    ListPage *field_17B0;
} ListOwner;

extern void *D_08A23FF0;
extern void func_08A18CE0(ListOwner *self);
extern void func_088D46E4(ListScroller *scroller, int first, int zero, int last, int none, int mode);
extern void func_08A18E08(ListOwner *self);
extern void func_08A1E1C8(void *renderer, int page, int value);

void func_08A18D70(ListOwner *self)
{
    ListPage *page;
    ListScroller *scroller;
    func_08A18CE0(self);
    page = self->field_17B0;
    scroller = &page->field_16DC;
    func_088D46E4(scroller, page->field_1680[page->field_16B4].field_00, 0,
                  page->field_1680[page->field_16B4].field_04 - 1, 0, -1);
    scroller = &self->field_17B0->field_16DC;
    scroller->field_00 = scroller->field_00 | 1;
    func_08A18E08(self);
    func_08A1E1C8(D_08A23FF0, self->field_17B0->field_16B4 + 2, self->field_17B0->field_10F8);
}
