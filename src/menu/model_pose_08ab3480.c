/* Fetches pose `index` (id from the table at D_08AB40B4) from the skeleton
   into a matrix and a value, then applies both to the model at +0x18.
   (Not a stack-spill function: the frame holds the two output locals.) */
typedef struct Matrix08AB3480 {
    int m[16];
} Matrix08AB3480;

typedef struct Scene08AB3480 {
    int unknown_00;
    void *skeleton;
    char unknown_08[0x10];
    void *model;
} Scene08AB3480;

typedef struct Menu08AB3480 {
    Scene08AB3480 *scene;
} Menu08AB3480;

extern unsigned short D_08AB40B4[];
void func_088DA0A8(void *skeleton, unsigned short id, Matrix08AB3480 *matrix, int *value);
void func_088D9B84(void *model, Matrix08AB3480 *matrix);
void func_088D9AF4(void *model, int *value);

void func_08AB3480(Menu08AB3480 *self, int index)
{
    Matrix08AB3480 matrix;
    int value;
    func_088DA0A8(self->scene->skeleton, D_08AB40B4[index], &matrix, &value);
    func_088D9B84(self->scene->model, &matrix);
    func_088D9AF4(self->scene->model, &value);
}
