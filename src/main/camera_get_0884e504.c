typedef struct CameraParams {
    char pad_00[0x44];
    float field_44;
} CameraParams;

float func_0884E504(CameraParams *self)
{
    return self->field_44;
}
