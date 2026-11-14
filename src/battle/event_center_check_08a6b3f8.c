/* Steers the event towards the middle of the object's span: when the
 * owner is far enough from it, turns one way or the other (-1 no object,
 * -5/-6 when the turn is refused). */
typedef struct CenterEvent {
    unsigned char pad_00[0x64];
    void *owner;
} CenterEvent;

extern float D_08A7AC08;
extern float func_08AB33F8(void *object);
extern float func_08AB33DC(void *object);
extern void *func_08AB328C(void *owner);
extern float func_08A7B468(void *body);
extern float func_089A2C24(float value);
extern int func_08A6B23C(CenterEvent *self);
extern int func_08A6B29C(CenterEvent *self);

int func_08A6B3F8(CenterEvent *self, void *object)
{
    float first;
    float middle;
    float position;
    if (object == 0) {
        return -1;
    }
    first = func_08AB33F8(object);
    middle = (first + func_08AB33DC(object)) * 0.5f;
    position = func_08A7B468(func_08AB328C(self->owner));
    float distance = func_089A2C24(middle - position);
    if (distance > D_08A7AC08) {
        if (middle < position) {
            if (func_08A6B23C(self) != 0) {
                return -5;
            }
        } else {
            if (func_08A6B29C(self) != 0) {
                return -6;
            }
        }
    }
    return 0;
}
