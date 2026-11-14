typedef struct CameraParams {
    char pad_00[0x48];
    float field_48;
} CameraParams;

float func_0884E4F4(CameraParams *self)
{
    return self->field_48;
}
