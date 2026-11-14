#include "menu/agent29_types.h"

/* Arcade course view, mode 1: updates its parts; once the part before the
   current page is done, refreshes the current one and moves on. */
extern "C" {
void func_089EAB70(CourseView29 *view);

void func_089EAA7C(CourseView29 *view, float time)
{
    for (int i = 0; i < 3; i++) {
        ViewPart29 *part = view->parts[i];
        if (part != 0) {
            part->Update(time);
            if (view->page > 0 && view->page == i && view->parts[i - 1]->IsDone() == 1) {
                part->Refresh();
                view->page++;
            }
        }
    }
    func_089EAB70(view);
}
}
