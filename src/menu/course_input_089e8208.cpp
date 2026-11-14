#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): course chooser input. Back closes the list,
   confirm picks the course (state 5, then 0xC or 2), the last mask opens
   it again; the list is redrawn every frame. */
extern "C" {
extern unsigned long long D_089E9CE0;
extern unsigned long long D_089E9CE8;
extern unsigned long long D_089E9CF0;
extern unsigned long long D_089E9CF8;
void func_089EEB84(void *list);
void func_089EEB4C(void *list);
void func_089EEA60(void *list);

void func_089E8208(ScreenE9D38 *screen)
{
    ScreenE9D38Ex *self = (ScreenE9D38Ex *)screen;
    Pad29 *pad = func_088729A0(0);
    ArcadeCourse29 *course = &self->course;
    int next;

    if (pad->Pressed(D_089E9CE0) != 0 || pad->Pressed(D_089E9CE8) != 0) {
        func_089EEB84(course->list);
        func_088D4BF4(0);
    } else if (pad->Pressed(D_089E9CF0) != 0) {
        func_089EEB4C(course->list);
        course->state = 5;
        if (*(int *)((char *)course->list + 0x1C) == 0) {
            func_088D4BF4(0x11);
            course->next = 0xC;
        } else {
            next = 2;
            func_088D4BF4(2);
            course->next = next;
        }
    } else if (pad->Pressed(D_089E9CF8) != 0) {
        next = 2;
        func_088D4BF4(2);
        func_089EEB4C(course->list);
        course->state = 5;
        course->next = next;
    }
    func_089EEA60(course->list);
}
}
