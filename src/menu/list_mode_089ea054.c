/* Arcade course list: starts the picture model and takes the mode. */
typedef struct ListMode29 {
    int value;
} ListMode29;

typedef struct CourseList29 {
    void *handles[2];
    void *field_08;
    void *field_0C;
    ListMode29 mode;
    int field_14;
} CourseList29;

extern void *func_088DAA40(void *handle, int arg1, int arg2);

void func_089EA054(CourseList29 *list, ListMode29 *mode)
{
    list->field_0C = func_088DAA40(list->handles[1], 0, 0);
    list->mode = *mode;
    list->field_14 = 0;
}
