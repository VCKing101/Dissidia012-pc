/* Resolves the parent and attaches a new node to it, or returns 0. */
int func_088713E4(int scene, int parent_id);
int func_08871718(int scene, int parent, int a, int b, int c, int d, int one);

int func_088715BC(int scene, int parent_id, int a, int b, int c, int d)
{
    int parent = func_088713E4(scene, parent_id);
    if (parent == 0) {
        return 0;
    }
    return func_08871718(scene, parent, a, b, c, d, 1);
}
