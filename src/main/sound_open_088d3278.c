/* Replaces the handle's sound with a newly opened one. */
typedef struct SoundRef088D3278 {
    int sound;
} SoundRef088D3278;

void func_088D3334(SoundRef088D3278 *ref);
int func_0887B3EC(int a, int b, int c, int d, int e, int f);

void func_088D3278(SoundRef088D3278 *ref, int a, int b, int d, int e, int f)
{
    func_088D3334(ref);
    ref->sound = func_0887B3EC(a, b, 0, d, e, f);
}
