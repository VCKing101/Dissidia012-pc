/* Plays a cue on the effect's voice when the voice is ready. */
typedef struct Fx088A6CB8 {
    char pad_00[0xCC];
    void *voice;
} Fx088A6CB8;

int func_088A751C(Fx088A6CB8 *fx);
void func_088A4540(void *voice, int cue, float volume);

void func_088A6CB8(Fx088A6CB8 *fx, int cue, float volume)
{
    if (func_088A751C(fx) == 1) {
        func_088A4540(fx->voice, cue, volume);
    }
}
