/* Releases the pull's sound object. */
typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;
    int field_0C;
    void *sound;     /* +0x10 */
} PullState29;

extern PullState29 D_09B6DA40;
extern void func_0886278C(void *object);

void func_089FA658(void)
{
    PullState29 *pull = &D_09B6DA40;
    func_0886278C(pull->sound);
    pull->sound = 0;
}
