typedef struct CameraData48678 {
    char pad_000[0x24];
    float field_24;
    float field_28;
} CameraData48678;

typedef struct CameraOwner48678 {
    char pad_000[0x14];
    CameraData48678 *data;
} CameraOwner48678;

typedef struct CameraSystem48678 {
    char pad_000[0xC0];
    char camera[0x4];
} CameraSystem48678;

extern CameraSystem48678 *D_08B3D6B8;
extern void func_08935828(void *dst, int value, int size);
extern float func_0887357C(void *camera);
extern float func_08873584(void *camera);
extern void func_08873594(void *camera, float a, float b);

void func_08A48678(CameraOwner48678 *self, CameraData48678 *data)
{
    void *camera;
    self->data = data;
    func_08935828(data, 0, 0x98);
    camera = D_08B3D6B8->camera;
    self->data->field_24 = func_0887357C(camera);
    self->data->field_28 = func_08873584(camera);
    func_08873594(D_08B3D6B8->camera, 0.3f, self->data->field_28);
}
