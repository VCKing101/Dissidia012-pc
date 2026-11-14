/* Replaces the handle's sound with the named sound. */
typedef struct SoundRef088D321C {
    int sound;
} SoundRef088D321C;

extern int D_089D6998;

void func_088D3334(SoundRef088D321C *ref);
int func_0887D868(int system, int a, int name, int b, int c, int d, int e, int f);

void func_088D321C(SoundRef088D321C *ref, int name)
{
    func_088D3334(ref);
    ref->sound = func_0887D868(D_089D6998, 0, name, -1, 0, 0, 0, 0);
}
