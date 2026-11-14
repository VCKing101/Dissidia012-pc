/* Clears the world's two pending counters and resets its scene. */
struct World08841798 {
    void *scene;
    char pad_04[0x3C - 4];
    int pending_a;
    int pending_b;
};

extern "C" {

extern World08841798 *D_089D4958;

void func_088825B4(void *scene);

void func_08841798(void)
{
    World08841798 *w = D_089D4958;
    w->pending_b = 0;
    w->pending_a = 0;
    func_088825B4(w->scene);
}

}
