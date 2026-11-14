/* When flag bit 0 is set, fetches pose `index` (id from the table at
   D_08AA8838) from the scene's skeleton into a matrix and a value, then
   applies both to the model at +0x28.
   (Not a stack-spill function: the frame holds the two output locals.) */
typedef struct Matrix08AA4C60 {
    int m[16];
} Matrix08AA4C60;

typedef struct Scene08AA4C60 {
    char unknown_0000[0x20];
    void *skeleton;
    int unknown_24;
    void *model;
    char unknown_002C[0x1E44];
    unsigned flags;
} Scene08AA4C60;

typedef struct Menu08AA4C60 {
    char unknown_0000[0x1EE4];
    Scene08AA4C60 *scene;
} Menu08AA4C60;

extern unsigned short D_08AA8838[];
void func_088DA0A8(void *skeleton, unsigned short id, Matrix08AA4C60 *matrix, int *value);
void func_088D9B84(void *model, Matrix08AA4C60 *matrix);
void func_088D9AF4(void *model, int *value);

void func_08AA4C60(Menu08AA4C60 *self, int index)
{
    Matrix08AA4C60 matrix;
    int value;
    if ((self->scene->flags & 1) != 0) {
        func_088DA0A8(self->scene->skeleton, D_08AA8838[index], &matrix, &value);
        func_088D9B84(self->scene->model, &matrix);
        func_088D9AF4(self->scene->model, &value);
    }
}
