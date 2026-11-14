/* Opens the screen's layer with the owner's resource and a 0x20 size; state 4. */
typedef struct LayerSource {
    int field_00;
    int field_04;
} LayerSource;

typedef struct LayerOwner {
    char pad_00[0x8];
    LayerSource field_08;
} LayerOwner;

typedef struct LayerRequest {
    int field_00;
    LayerOwner *field_04;
    int field_08;
    int field_0C;
} LayerRequest;

typedef struct LayerSlot {
    int field_00;
    void *field_04;
    int field_08;
} LayerSlot;

typedef struct LayerScreen {
    char pad_000[0x13C];
    LayerOwner *field_13C;
    char pad_140[0xC];
    LayerSlot field_14C;
} LayerScreen;

extern void func_089EE898(void *layer, LayerRequest *request);
extern void func_089EEB08(void *layer);
extern void func_089EEA60(void *layer);

void func_089E81A4(LayerScreen *self)
{
    LayerRequest request;
    LayerSlot *slot = &self->field_14C;
    LayerOwner *owner = self->field_13C;
    LayerSource *source = &owner->field_08;
    request.field_00 = source->field_04;
    request.field_04 = owner;
    request.field_08 = 0;
    request.field_0C = 0x20;
    func_089EE898(slot->field_04, &request);
    func_089EEB08(slot->field_04);
    func_089EEA60(slot->field_04);
    slot->field_08 = 4;
}
