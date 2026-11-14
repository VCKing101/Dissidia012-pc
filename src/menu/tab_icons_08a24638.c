/* Sets the tab icon frame from the current tab's flag and refreshes both tab icons. */
typedef struct TabEntry {
    unsigned char field_00;
    char pad_01[0x7];
} TabEntry;

typedef struct TabPage {
    char pad_0000[0x510];
    void *field_510;
    void *field_514;
    char pad_0518[0x2048 - 0x518];
    int field_2048;
    char pad_204C[0x25AC - 0x204C];
    TabEntry field_25AC[1];
} TabPage;

typedef struct TabOwner {
    char pad_0000[0x28F0];
    TabPage *field_28F0;
} TabOwner;

extern void func_088D9F5C(void *icon, int frame, int one);
extern void func_088D9CD8(void *icon);
extern void func_08A24A60(TabOwner *self);
extern void func_08A246F4(TabOwner *self);

void func_08A24638(TabOwner *self)
{
    TabPage *page = self->field_28F0;
    TabEntry *entry = &page->field_25AC[page->field_2048];
    int frame = (entry->field_00 & 1) != 0 ? 2 : 3;
    func_088D9F5C(page->field_514, frame, 1);
    func_088D9CD8(self->field_28F0->field_510);
    func_088D9CD8(self->field_28F0->field_514);
    func_08A24A60(self);
    func_08A246F4(self);
}
