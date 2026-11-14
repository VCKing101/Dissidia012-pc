/* Replaces the handle's sound with a copy of another handle's sound. */
typedef struct SoundRef088D32F8 {
    int sound;
} SoundRef088D32F8;

void func_088D3334(SoundRef088D32F8 *ref);
int func_0887E534(int sound);

void func_088D32F8(SoundRef088D32F8 *ref, SoundRef088D32F8 *from)
{
    func_088D3334(ref);
    ref->sound = func_0887E534(from->sound);
}
