typedef struct TurnFighter {
    char pad_000[0x2FC];
    void *target;
    void *fallback;
} TurnFighter;

extern float D_08A56B50;
extern void func_08835F60(TurnFighter *self, void *target, float angle);
extern void func_08834C10(TurnFighter *self, int action, int flags);

/* Turns towards the target (or the fallback target), then starts action 5. */
void func_08A56A5C(TurnFighter *self)
{
    if (self->target != 0) {
        func_08835F60(self, self->target, D_08A56B50 / 180.0f * 180.0f);
    } else if (self->fallback != 0) {
        func_08835F60(self, self->fallback, D_08A56B50 / 180.0f * 180.0f);
    }
    func_08834C10(self, 5, 0);
}
