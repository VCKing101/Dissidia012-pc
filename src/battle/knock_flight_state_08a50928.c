/* Fighter state callback, burst knock flight: keeps the knock effect on the
 * fighter, turned to its facing, and lands it unless a fall motion is
 * already playing. */
typedef struct FlightVec4 {
    float x, y, z, w;
} FlightVec4;

typedef struct FlightMove {
    unsigned char pad_00[0x20];
    void *effect;
} FlightMove;

typedef struct FlightFighter {
    unsigned char pad_000[0x80];
    FlightVec4 pos;
    FlightVec4 facing;
    unsigned char pad_0a0[0xE0];
    short motion_id;
    unsigned char pad_182[0x226];
    FlightMove *move;
} FlightFighter;

extern unsigned char D_09AF8AB0[];

extern void func_088A7284(void *effect, float x, float y, float z);
extern float func_08868128(void *frame, FlightVec4 *dir, int flags);
extern void func_088A72B4(void *effect, float x, float y, float z);
extern void func_08A507FC(void *ctx, FlightFighter *fighter);

void func_08A50928(void *ctx, FlightFighter *fighter)
{
    FlightMove *move = fighter->move;
    short id;
    if (move->effect != 0) {
        func_088A7284(move->effect, fighter->pos.x, fighter->pos.y, fighter->pos.z);
        func_088A72B4(move->effect, 0.0f, func_08868128(D_09AF8AB0, &fighter->facing, 0), 0.0f);
    }
    id = fighter->motion_id;
    if (id != 0xF0 && id != 0xF2 && id != 0xF4 && id != 0xF3 && id != 0x65) {
        func_08A507FC(ctx, fighter);
    }
}
