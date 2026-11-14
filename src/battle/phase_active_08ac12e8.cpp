typedef struct PhaseState {
    unsigned char active;
    char pad_01[0xF];
    int phase;
} PhaseState;

extern "C" {

/* 1 in phases 1 to 3. */
unsigned char func_08AC12E8(PhaseState *self)
{
    if (self->active == 0) {
        return 0;
    }
    int result = 0;
    if (self->phase == 1 || self->phase == 2 || self->phase == 3) {
        result = 1;
    }
    return result;
}

}
