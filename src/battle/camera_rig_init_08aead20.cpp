typedef struct CameraRig {
    float x;
    float y;
    int mode;
    char pad_00C[0x900];
    float offset_x;
    float offset_y;
    float offset_z;
    int field_918;
    int axis_a;
    int axis_b;
    int field_924;
    int field_928;
    unsigned char enabled;

    static CameraRig *init(CameraRig *self)
    {
        self->mode = 0;
        self->x = 0.0f;
        self->y = 0.0f;
        self->offset_x = 0.0f;
        self->offset_y = 0.0f;
        self->offset_z = 0.0f;
        self->field_918 = 0;
        self->axis_a = 2;
        self->axis_b = 3;
        self->field_924 = 0;
        self->field_928 = 0;
        self->enabled = 1;
        return self;
    }
} CameraRig;

extern "C" CameraRig *func_08AEAD20(CameraRig *self)
{
    return CameraRig::init(self);
}
