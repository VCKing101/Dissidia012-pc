/* Opens the list on the source's entries (window 0..0xBC) and marks the panel open. */
typedef struct OpenSource {
    char pad_00[0x34];
    char field_34[0x10];
} OpenSource;

typedef struct OpenOwner {
    char pad_000[0x148];
    OpenSource field_148;
} OpenOwner;

typedef struct OpenRequest {
    void *field_00;
    short field_04;
    short field_06;
    short field_08;
    unsigned char field_0A;
} OpenRequest;

typedef struct OpenPanel {
    char pad_00[0x28];
    int field_28;
    int field_2C;
} OpenPanel;

extern void func_08A72848(OpenPanel *self, OpenSource *source, OpenRequest *request);
extern void func_08A72ABC(OpenPanel *self);
extern void func_08A72A14(OpenPanel *self);

void func_08A772FC(OpenPanel *self, OpenOwner *owner)
{
    OpenRequest request;
    OpenSource *source = &owner->field_148;
    request.field_00 = source->field_34;
    request.field_04 = 0;
    request.field_06 = 0xBC;
    request.field_08 = 1;
    request.field_0A = 1;
    func_08A72848(self, source, &request);
    func_08A72ABC(self);
    func_08A72A14(self);
    self->field_28 = 1;
    self->field_2C = 1;
}
