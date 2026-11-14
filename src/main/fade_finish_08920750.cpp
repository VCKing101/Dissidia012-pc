/* Advances a fade that is in state 1; returns whether it did. */
extern "C" {

typedef struct FadeState {
    int state;                  /* 0x00 */
} FadeState;

extern void func_08920788(FadeState *fade);

bool func_08920750(FadeState *fade)
{
    if (fade->state == 1) {
        func_08920788(fade);
        return true;
    } else {
        return false;
    }
}

}
