/* Constructor: base init, then the listener record past 0x14C68 and the camera state. */
struct BattleView {
    char pad_00000[0x14C68];
    void *listener_vtable;
    BattleView *listener_owner;
    void *track;
    char pad_14C74[0x31968 - 0x14C74];
    int camera_mode;
    int camera_target;

    static void init(BattleView *view, void *track, void *vt)
    {
        view->listener_vtable = vt;
        view->listener_owner = view;
        view->track = track;
        view->camera_mode = 0;
        view->camera_target = -1;
    }
};

extern "C" {
extern char D_089CCDC8[];
extern void func_089096B0(BattleView *view);

BattleView *func_088F7834(BattleView *view, void *track)
{
    func_089096B0(view);
    BattleView::init(view, track, D_089CCDC8);
    return view;
}
}
