// Divides the two offsets by the selected image's width and height, adds the node's origin, and calls func_089A0F84.
struct Sz { int w; int h; };
struct Img { char d[0x1C]; struct Sz sz; };
struct Hold { struct Img *img; };
struct N { struct Hold **tbl; char d[0x24]; float ox; float oy; };
extern void func_089A0F84(float x, float y);
void func_08973508(float x, float y, struct N *n, int idx)
{
    struct Sz *s = &n->tbl[idx]->img->sz;
    func_089A0F84(x / (float)s->w + n->ox, y / (float)s->h + n->oy);
}
