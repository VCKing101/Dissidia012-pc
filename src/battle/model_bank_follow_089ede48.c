#include "battle/agent28_types.h"

/* Places the bank's frame over its owner's head on screen and attaches the
 * icon to the frame. */
typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct FollowBody {
    unsigned char pad_00[0x80];
    Vec4 head;
} FollowBody;

typedef struct FollowFighter {
    unsigned char pad_0000[0x1108];
    FollowBody *body;
} FollowFighter;

typedef struct Matrix44 {
    float m[16];
} Matrix44;

typedef struct Rgba {
    unsigned char r, g, b, a;
} Rgba;

extern float D_089EE960;
extern FollowFighter *func_089E95B0(int side);
extern int func_08866E04(Vec4 *screen, Vec4 *world);
extern void func_088D9C60(void *model, int x, int y);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_089EDE48(ModelBank *bank)
{
    FollowFighter *fighter;
    FollowBody *body;
    Vec4 pos;

    if (bank->frame == 0) {
        return 0;
    }
    fighter = func_089E95B0(bank->owner);
    if (fighter == 0) {
        return -1;
    }
    body = fighter->body;
    if (body == 0) {
        return -2;
    }
    pos = body->head;
    pos.y += 2.0f;
    if (func_08866E04(&pos, &pos) == 0) {
        return -4;
    }
    func_088D9C60(bank->frame, (int)pos.x, (int)(pos.y - D_089EE960));
    if (bank->icon == 0) {
        return 0;
    }
    {
        Matrix44 matrix;
        Rgba color;
        func_088DA0A8(bank->frame, 0, &matrix, &color);
        func_088D9B84(bank->icon, &matrix);
        func_088D9AF4(bank->icon, &color);
    }
    return 0;
}
