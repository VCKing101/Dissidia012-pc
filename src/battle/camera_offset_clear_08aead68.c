typedef struct CameraRig {
    char pad_000[0x90C];
    float offset_x;
    float offset_y;
    float offset_z;
} CameraRig;

void func_08AEAD68(CameraRig *self)
{
    self->offset_z = 0.0f;
    self->offset_x = 0.0f;
    self->offset_y = 0.0f;
}
