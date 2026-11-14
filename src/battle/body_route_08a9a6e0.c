typedef struct UnitInfo {
    char pad_00[0x4];
    void *body;
} UnitInfo;

typedef struct RouteOwner {
    unsigned char active;
    char pad_01[0x3];
    int unit;
} RouteOwner;

extern UnitInfo *func_08AB3264(int unit);
extern int func_08837908(void *body);
extern int func_088378E8(void *body);
extern int func_08A9A79C(RouteOwner *self);
extern int func_08A9A968(RouteOwner *self);

int func_08A9A6E0(RouteOwner *self)
{
    void *body;
    if (!self->active) {
        return -1;
    }
    body = func_08AB3264(self->unit)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08837908(body) == 1) {
        if (func_088378E8(body) == 1) {
            if (func_08A9A79C(self) != 0) {
                return -3;
            }
        } else {
            if (func_08A9A968(self) != 0) {
                return -4;
            }
        }
    }
    return 0;
}
