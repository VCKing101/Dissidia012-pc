/* Constructor: base part, vtable and the shared page pointers of the panel body. */
typedef struct PanelPageView {
    char pad_000[0x400];
    char field_400[0x520];
    int field_920;
} PanelPageView;

typedef struct PanelPageData {
    char pad_000[0x14C];
    PanelPageView *field_14C;
} PanelPageData;

typedef struct PanelBody {
    PanelPageView *field_00;
    char *field_04;
    int field_08;
    int field_0C[4];
    char pad_01C[0x3E4 - 0x1C];
    int field_3E4;
    int field_3E8;
} PanelBody;

typedef struct PanelObject {
    void *field_00;
    PanelBody field_04;
    char pad_3F0[0x418 - 0x3F0];
    PanelBody *field_418;
} PanelObject;

extern char D_08A47D90[];
extern PanelPageData *D_08A457EC;
extern void func_08A408A0(PanelObject *self);

PanelObject *func_08A4718C(PanelObject *self)
{
    PanelBody *body;
    int i;
    func_08A408A0(self);
    self->field_00 = D_08A47D90;
    body = &self->field_04;
    self->field_418 = body;
    body->field_00 = D_08A457EC->field_14C;
    self->field_418->field_04 = D_08A457EC->field_14C->field_400;
    self->field_418->field_08 = D_08A457EC->field_14C->field_920;
    for (i = 0; i < 4; i++) {
        self->field_418->field_0C[i] = 0;
    }
    self->field_418->field_3E4 = 0;
    self->field_418->field_3E8 = 0;
    return self;
}
