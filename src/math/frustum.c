#include "frustum.h"

#include <math.h>

static void normalize_plane(float p[4]) {
    float l = sqrtf(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
    if (l < 1e-6f) return;
    for (int i = 0; i < 4; i++) p[i] /= l;
}

void frustum_from_matrix(frustum *f, mat4 m) {
    // use gribb-hartmann to pull planes from row sums of the view-proj
    // layout note: our mat4 is column-major, so m.m[col][row]
    // left
    f->planes[0][0] = m.m[0][3] + m.m[0][0];
    f->planes[0][1] = m.m[1][3] + m.m[1][0];
    f->planes[0][2] = m.m[2][3] + m.m[2][0];
    f->planes[0][3] = m.m[3][3] + m.m[3][0];
    // right
    f->planes[1][0] = m.m[0][3] - m.m[0][0];
    f->planes[1][1] = m.m[1][3] - m.m[1][0];
    f->planes[1][2] = m.m[2][3] - m.m[2][0];
    f->planes[1][3] = m.m[3][3] - m.m[3][0];
    // bottom
    f->planes[2][0] = m.m[0][3] + m.m[0][1];
    f->planes[2][1] = m.m[1][3] + m.m[1][1];
    f->planes[2][2] = m.m[2][3] + m.m[2][1];
    f->planes[2][3] = m.m[3][3] + m.m[3][1];
    // top
    f->planes[3][0] = m.m[0][3] - m.m[0][1];
    f->planes[3][1] = m.m[1][3] - m.m[1][1];
    f->planes[3][2] = m.m[2][3] - m.m[2][1];
    f->planes[3][3] = m.m[3][3] - m.m[3][1];
    // near
    f->planes[4][0] = m.m[0][3] + m.m[0][2];
    f->planes[4][1] = m.m[1][3] + m.m[1][2];
    f->planes[4][2] = m.m[2][3] + m.m[2][2];
    f->planes[4][3] = m.m[3][3] + m.m[3][2];
    // far
    f->planes[5][0] = m.m[0][3] - m.m[0][2];
    f->planes[5][1] = m.m[1][3] - m.m[1][2];
    f->planes[5][2] = m.m[2][3] - m.m[2][2];
    f->planes[5][3] = m.m[3][3] - m.m[3][2];

    for (int i = 0; i < 6; i++) normalize_plane(f->planes[i]);
}

int frustum_contains_aabb(const frustum *f, aabb a) {
    for (int i = 0; i < 6; i++) {
        const float *p = f->planes[i];
        // pick the positive vertex (p-vertex) for this plane
        vec3 v = {
            p[0] >= 0 ? a.max.x : a.min.x,
            p[1] >= 0 ? a.max.y : a.min.y,
            p[2] >= 0 ? a.max.z : a.min.z
        };
        if (p[0] * v.x + p[1] * v.y + p[2] * v.z + p[3] < 0) return 0;
    }
    return 1;
}
