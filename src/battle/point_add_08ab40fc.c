typedef struct Point3 {
    float x;
    float y;
    float z;
} Point3;

typedef struct PointList {
    char pad_00[0xC];
    unsigned int count;
    Point3 points[4];
} PointList;

extern int func_08AB4190(PointList *self);

int func_08AB40FC(PointList *self, float x, float y, float z)
{
    unsigned int count;
    if (func_08AB4190(self) == 0) {
        return -1;
    }
    count = self->count;
    if (count >= 4) {
        return -2;
    }
    self->points[count].x = x;
    self->points[count].y = y;
    self->points[count].z = z;
    self->count = count + 1;
    return 0;
}
