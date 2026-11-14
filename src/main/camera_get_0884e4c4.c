typedef struct CameraParams {
    char pad_00[0x24];
    float field_24;
} CameraParams;

float func_0884E4C4(CameraParams *self)
{
    return self->field_24;
}
