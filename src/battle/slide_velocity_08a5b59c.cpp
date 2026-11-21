/* Slide velocity: none while turning (motion 0x66) or stopped, otherwise
 * the slide push for the track's lean. */
struct SlideVelVec4 {
    float x, y, z, w;
};

struct SlideVelTrack {
    unsigned char pad_00[0x28];
    float lean;
};

struct SlideVelHolder {
    unsigned char pad_00[0x10];
    SlideVelTrack track;
};

struct SlideVelFighter {
    unsigned char pad_000[0x180];
    short motion_id;
    unsigned char pad_182[0x192];
    SlideVelHolder *holder;
};

extern SlideVelVec4 D_09AF84C0;
extern float D_08A5C304;
extern float D_08A5C308;

extern "C" {

extern int func_0883752C(SlideVelFighter *fighter);
extern SlideVelVec4 func_08A5BC58(void *ctx, SlideVelFighter *fighter, float speed, float lean);

SlideVelVec4 func_08A5B59C(void *ctx, SlideVelFighter *fighter)
{
    SlideVelTrack *track = &fighter->holder->track;
    if (fighter->motion_id == 0x66 || func_0883752C(fighter) != 0) {
        return D_09AF84C0;
    }
    return func_08A5BC58(ctx, fighter, D_08A5C304, D_08A5C308 + track->lean);
}

}
