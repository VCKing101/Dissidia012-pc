/* Stops the state's voice: -1 when inactive, -2 when the stop fails. */
typedef struct State088191A0 {
    char pad_00[0xC];
    unsigned char active;
    char pad_0D[7];
    void *voice;
} State088191A0;

int func_08815D90(void *voice);

int func_088191A0(State088191A0 *s)
{
    if (s->active == 0) {
        return -1;
    }
    if (s->voice == 0) {
        return 0;
    }
    if (func_08815D90(s->voice) != 0) {
        return -2;
    }
    return 0;
}
