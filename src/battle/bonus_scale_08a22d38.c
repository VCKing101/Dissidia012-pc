/* Returns the battle's bonus scale, or zero when the bonus does not apply:
 * no active entry, the gate is set, the mode is not 3 or the check passes. */
typedef struct GateState {
    unsigned char pad_000[0x278];
    int field_278;
} GateState;

typedef struct ModeState {
    unsigned char pad_00[0x48];
    int mode;
} ModeState;

extern void *D_08A22AB0;
extern GateState *D_08A405E0;
extern ModeState *D_089D6A10;
extern float D_089D5EAC;
extern void *func_08A22970(void *table);
extern void *func_08A074FC(void *entry);
extern int func_08A15A70(void *entry);
extern int func_08A230C8(void);

float func_08A22D38(void)
{
    if (func_08A15A70(func_08A074FC(func_08A22970(D_08A22AB0))) == 0) {
        return 0.0f;
    }
    if ((unsigned char)(D_08A405E0->field_278 ? 1 : 0) == 1) {
        return 0.0f;
    }
    if ((unsigned char)(D_089D6A10->mode == 3) == 0) {
        return 0.0f;
    }
    if (func_08A230C8() == 1) {
        return 0.0f;
    }
    return D_089D5EAC;
}
