typedef struct CameraData47390 {
    char pad_000[0x34];
    float field_34;
    float field_38;
} CameraData47390;

typedef struct CameraOwner47390 {
    char pad_000[0x14];
    CameraData47390 *data;
} CameraOwner47390;

typedef struct CameraSystem47390 {
    char pad_000[0xC0];
    char camera[0x4];
} CameraSystem47390;

extern CameraSystem47390 *D_08B3D6B8;
extern void func_08935828(void *dst, int value, int size);
extern float func_0887357C(void *camera);
extern float func_08873584(void *camera);
extern void func_08873594(void *camera, float a, float b);

void func_08A47390(CameraOwner47390 *self, CameraData47390 *data)
{
    void *camera;
    self->data = data;
    func_08935828(data, 0, 0x64);
    camera = D_08B3D6B8->camera;
    self->data->field_34 = func_0887357C(camera);
    self->data->field_38 = func_08873584(camera);
    func_08873594(D_08B3D6B8->camera, 0.3f, self->data->field_38);
}
