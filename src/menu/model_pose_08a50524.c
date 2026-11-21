/* Picks the pose of the screen's model by the global mode (0/1 or 2) and
 * applies it. */
typedef struct Work50524 {
    char pad_00[0x8];
    char field_08[0x4];       /* +0x08 */
} Work50524;

typedef struct Screen50524 {
    char pad_00[0x58];
    Work50524 *work;          /* +0x58 */
} Screen50524;

typedef struct Pose50524 {
    char data[0xC];
} Pose50524;

extern void *D_089D8508;
extern int D_09B6DD00;
void func_0891ACC8(Pose50524 *pose, void *source, int a, int b);
void func_0891B8C0(void *object, Pose50524 *pose);
void func_0891B96C(void *object, int a, int b);

void func_08A50524(Screen50524 *self)
{
    void *object = D_089D8508;
    int a = 1;
    int b = 0;
    Pose50524 pose;

    switch (D_09B6DD00) {
    case 0:
    case 1:
        a = 0;
        b = 1;
        break;
    case 2:
        a = 1;
        b = 0;
        break;
    }
    func_0891ACC8(&pose, self->work->field_08, a, b);
    func_0891B8C0(object, &pose);
    func_0891B96C(object, 0, 0);
}
