typedef struct CameraData45658 {
    char pad_000[0x2E4];
    float field_2E4;
    float field_2E8;
} CameraData45658;

typedef struct CameraOwner45658 {
    char pad_000[0x14];
    CameraData45658 *data;
} CameraOwner45658;

typedef struct CameraSystem45658 {
    char pad_000[0xC0];
    char camera[0x4];
} CameraSystem45658;

extern CameraSystem45658 *D_08B3D6B8;
extern void func_08935828(void *dst, int value, int size);
extern float func_0887357C(void *camera);
extern float func_08873584(void *camera);

void func_08A45658(CameraOwner45658 *self, CameraData45658 *data)
{
    void *camera;
    self->data = data;
    func_08935828(data, 0, 0x340);
    camera = D_08B3D6B8->camera;
    self->data->field_2E4 = func_0887357C(camera);
    self->data->field_2E8 = func_08873584(camera);
}
