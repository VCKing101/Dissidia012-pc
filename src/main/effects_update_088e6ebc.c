/* Pushes the effect state into the global renderer. */
typedef struct EffectState {
    char pad_000[0x410];
    float level;
} EffectState;

extern char *D_089D5EC0;
extern char D_09AF84E0[];
extern void func_0885EAE0(void *target, void *source);
extern void func_0887358C(void *target, float level);

void func_088E6EBC(EffectState *fx)
{
    func_0885EAE0(D_089D5EC0 + 0x720, D_09AF84E0);
    func_0887358C(D_089D5EC0 + 0x7E0, fx->level);
}
