typedef struct CameraParams {
    char pad_00[0x20];
    float fov_deg;
    float field_24;
    float field_28;
    char pad_2C[0x18];
    float field_44;
    float field_48;
} CameraParams;

extern float D_089C8AB8;

float func_0884E494(CameraParams *self)
{
    return D_089C8AB8 / 180.0f * self->fov_deg;
}

float func_0884E4B4(CameraParams *self)
{
    return self->field_28;
}
