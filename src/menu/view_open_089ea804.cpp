/* Arcade course view: plays the open motions of its models, shows its
   three parts, updates the first and moves to the next page. */
struct ViewPart29 {
    virtual void unknown_00();
    virtual void unknown_01();
    virtual void unknown_02();
    virtual void Show();
    virtual void unknown_04();
    virtual void unknown_05();
    virtual void Refresh();
};

struct CourseView29 {
    char pad_00[0x14];
    void *frame;
    void *title;
    void *left;
    void *right;
    char pad_24[0xC];
    ViewPart29 *parts[3];
    int mode;
    int page;
};

extern "C" {
void func_088D9F5C(void *model, int motion, bool arg);
void func_089EAB70(CourseView29 *view);

void func_089EA804(CourseView29 *view)
{
    func_088D9F5C(view->frame, 0, true);
    *(short *)((char *)view->frame + 0xE) = 1;
    func_088D9F5C(view->title, 0, true);
    if (view->left != 0) {
        func_088D9F5C(view->left, 6, true);
    }
    if (view->right != 0) {
        func_088D9F5C(view->right, 3, true);
    }
    for (int i = 0; i < 3; i++) {
        if (view->parts[i] != 0) {
            view->parts[i]->Show();
        }
    }
    view->parts[0]->Refresh();
    view->page++;
    func_089EAB70(view);
    view->mode = 1;
}
}
