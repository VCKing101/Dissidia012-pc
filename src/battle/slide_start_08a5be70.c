/* Starts a slide at the given speed (capped): resets the slide track, aims
 * it at the fighter's target, and clears the slide flags. */
typedef struct SlideStartTrack {
    float x;
    float lean;
    float z;
    float speed;
    float end_x;
    float end_y;
    float scale_x;
    float scale_y;
    float scale_z;
    float pad_24;
    float turn;
    float rate;
    float time;
    unsigned int flags;
} SlideStartTrack;

typedef struct SlideStartHolder {
    unsigned char pad_00[0xC];
    float speed;
    SlideStartTrack track;
    unsigned char pad_48[0xAC];
    int count;
} SlideStartHolder;

typedef struct SlideStartFighter {
    unsigned char pad_000[0x314];
    SlideStartHolder *holder;
    unsigned char pad_318[0x6C];
    unsigned int flags;
} SlideStartFighter;

extern float D_089C7EA0;

extern void *func_08A5C24C(SlideStartFighter *fighter);
extern void func_089A4500(float speed);
extern float func_08837E50(SlideStartFighter *fighter, void *target, float speed);

void func_08A5BE70(SlideStartFighter *fighter, float speed)
{
    SlideStartTrack *track = &fighter->holder->track;
    void *target = func_08A5C24C(fighter);
    float zero;
    if (D_089C7EA0 < speed) {
        speed = D_089C7EA0;
    }
    func_089A4500(speed);
    zero = 0.0f;
    track->speed = speed;
    track->x = zero;
    track->z = zero;
    track->lean = func_08837E50(fighter, target, speed);
    track->end_x = zero;
    track->end_y = zero;
    track->scale_x = 1.0f;
    track->scale_y = 1.0f;
    track->scale_z = 1.0f;
    track->turn = zero;
    track->rate = 1.0f;
    track->time = zero;
    track->flags &= ~1;
    track->flags &= ~2;
    track->flags &= ~4;
    track->flags &= ~8;
    fighter->holder->count = 0;
    fighter->holder->speed = zero;
    fighter->flags &= ~0x400;
}
