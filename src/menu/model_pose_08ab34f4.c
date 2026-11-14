/* Fetches pose 5 from the first skeleton, or pose 3 from the second when
   `alternate` is set, and applies it to the player at +0x488.
   (Not a stack-spill function: the frame holds the two output locals.) */
typedef struct Matrix08AB34F4 {
    int m[16];
} Matrix08AB34F4;

typedef struct Scene08AB34F4 {
    int unknown_000;
    void *skeleton;
    void *alternate_skeleton;
    char unknown_00C[0x47C];
    void *player;
} Scene08AB34F4;

typedef struct Menu08AB34F4 {
    Scene08AB34F4 *scene;
} Menu08AB34F4;

void func_088DA0A8(void *skeleton, int id, Matrix08AB34F4 *matrix, int *value);
void func_0887B778(void *player, Matrix08AB34F4 *matrix);
void func_0887B828(void *player, int *value);
void func_0887B98C(void *player);
void func_0887B9A4(void *player);
void func_0887B9C4(void *player);

void func_08AB34F4(Menu08AB34F4 *self, int alternate)
{
    Matrix08AB34F4 matrix;
    int value;
    if (alternate == 0) {
        func_088DA0A8(self->scene->skeleton, 5, &matrix, &value);
    } else {
        func_088DA0A8(self->scene->alternate_skeleton, 3, &matrix, &value);
    }
    func_0887B778(self->scene->player, &matrix);
    func_0887B828(self->scene->player, &value);
    func_0887B98C(self->scene->player);
    if (alternate != 0) {
        func_0887B9C4(self->scene->player);
    } else {
        func_0887B9A4(self->scene->player);
    }
}
