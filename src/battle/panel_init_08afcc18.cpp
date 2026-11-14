typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct GaugePanel {
    int owner;
    int id;
    char pad_08[0xC8];
    int field_D0;
    int field_D4;
    char pad_D8[0x8];
    unsigned char field_E0;
    char pad_E1[0x1B];
    unsigned char field_FC;
    char pad_FD[0x3];
    Vec4 field_100;
    float values[5];
} GaugePanel;

extern "C" {

extern void func_089A3890(Vec4 *v);

void func_08AFCC18(GaugePanel *self, int owner, int id)
{
    self->owner = owner;
    self->id = id;
    self->field_D4 = 0;
    self->field_D0 = 0;
    self->field_E0 = 0;
    self->field_FC = 0;
    func_089A3890(&self->field_100);
    for (int i = 0; i < 5; i++) {
        self->values[i] = 0.0f;
    }
}

}
