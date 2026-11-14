/* Constructor: base part, vtable, sixteen icon slots and the shared page pointers. */
typedef struct IconPageView {
    char pad_000[0x400];
    char field_400[0x520];
    int field_920;
} IconPageView;

typedef struct IconPageData {
    char pad_000[0x14C];
    IconPageView *field_14C;
} IconPageData;

typedef struct IconLoaderBody {
    char pad_0000[0x8];
    IconPageView *field_08;
    char *field_0C;
    int field_10;
    int field_14[33];
    char pad_0098[0x490 - 0x98];
    char field_490[16][0x90];
    char pad_1390[0x1250 - 0xD90];
    int field_1250;
    int field_1254;
} IconLoaderBody;

typedef struct IconLoaderObject {
    void *field_00;
    IconLoaderBody field_04;
    char pad_125C[0x1274 - 0x125C];
    IconLoaderBody *field_1274;
} IconLoaderObject;

extern char D_08A3B718[];
extern IconPageData *D_08A457EC;
extern void func_08A408A0(IconLoaderObject *self);
extern void func_08993560(void *array, int count, int size, void *ctor);
extern void func_08A3A4F0(void *slot);

IconLoaderObject *func_08A3A518(IconLoaderObject *self)
{
    IconLoaderBody *body;
    int i;
    func_08A408A0(self);
    self->field_00 = D_08A3B718;
    func_08993560(self->field_04.field_490, 0x10, 0x90, func_08A3A4F0);
    body = &self->field_04;
    self->field_1274 = body;
    body->field_08 = D_08A457EC->field_14C;
    self->field_1274->field_0C = D_08A457EC->field_14C->field_400;
    self->field_1274->field_10 = D_08A457EC->field_14C->field_920;
    for (i = 0; i < 33; i++) {
        self->field_1274->field_14[i] = 0;
    }
    self->field_1274->field_1250 = 0;
    self->field_1274->field_1254 = 0;
    return self;
}
