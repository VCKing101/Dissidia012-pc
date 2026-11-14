/* Resumes the state's voice: -1 when inactive, -2 when the resume fails. */
typedef struct State088191F8 {
    char pad_00[0xC];
    unsigned char active;
    char pad_0D[7];
    void *voice;
} State088191F8;

int func_08815DD4(void *voice);

int func_088191F8(State088191F8 *s)
{
    if (s->active == 0) {
        return -1;
    }
    if (s->voice == 0) {
        return 0;
    }
    if (func_08815DD4(s->voice) != 0) {
        return -2;
    }
    return 0;
}
