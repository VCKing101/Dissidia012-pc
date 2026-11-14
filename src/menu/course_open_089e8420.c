#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): reads up to three stages of the course and
   opens the course view with them (state 7, then 6); state 2 when the
   course has none. */
typedef struct Stage29 {
    char pad_00[0x6];
    short field_06;
} Stage29;

typedef struct CourseRecord29 {
    char pad_00[0x8];
    short field_08;
} CourseRecord29;

typedef struct CourseInfo29 {
    int field_00;
    void *owner;
    int field_08;
    int field_0C;
    int count;
    int values[3];
    int field_20;
} CourseInfo29;

extern int func_0890119C(Stage29 **out, int count, int id, int arg);
extern CourseRecord29 *func_088B37C0(int id);
extern void func_089EE290(void *view, CourseInfo29 *info);
extern void func_089EE59C(void *view);
extern void func_089EE4F4(void *view);

void func_089E8420(ScreenE9D38 *screen)
{
    ScreenE9D38Ex *self = (ScreenE9D38Ex *)screen;
    ArcadeCourse29 *course = &self->course;
    Stage29 *stages[3];
    CourseInfo29 info;
    int count = func_0890119C(stages, 3, course->id, 1);
    int i;

    if (count > 0) {
        CourseRecord29 *record = func_088B37C0(course->id);
        PartE8E78 *owner = self->field_13C;
        info.field_00 = *(int *)&owner->pad_00[0x8];
        info.owner = owner;
        info.field_08 = 0;
        info.field_0C = record->field_08;
        info.count = count;
        info.field_20 = course->field_18;
        for (i = 0; i < count; i++) {
            info.values[i] = stages[i]->field_06;
        }
        func_089EE290(course->view, &info);
        func_089EE59C(course->view);
        func_089EE4F4(course->view);
        course->state = 7;
        course->field_10 = 6;
    } else {
        course->state = 2;
    }
}
