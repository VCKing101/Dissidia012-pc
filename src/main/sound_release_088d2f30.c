/* Releases the handle's sound. */
typedef struct SoundRef088D2F30 {
    int sound;
} SoundRef088D2F30;

void func_088DA060(int sound);

void func_088D2F30(SoundRef088D2F30 *ref)
{
    if (ref->sound != 0) {
        func_088DA060(ref->sound);
        ref->sound = 0;
    }
}
