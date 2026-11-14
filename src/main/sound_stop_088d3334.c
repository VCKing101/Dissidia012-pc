/* Stops and frees the handle's sound. */
typedef struct SoundRef088D3334 {
    int sound;
} SoundRef088D3334;

void func_0887B944(int sound);
void func_0887BA5C(int sound);

void func_088D3334(SoundRef088D3334 *ref)
{
    if (ref->sound != 0) {
        func_0887B944(ref->sound);
        func_0887BA5C(ref->sound);
        ref->sound = 0;
    }
}
