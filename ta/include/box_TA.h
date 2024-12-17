#ifndef BOX_TA_H
#define BOX_TA_H
#include "darknet_TA.h"

typedef struct{
    float dx, dy, dw, dh;
} dbox;

float box_rmse_TA(box a, box b);
box float_to_box_TA(float *f, int stride);
float box_iou_TA(box a, box b);
dbox diou_TA(box a, box b);
box decode_box_TA(box b, box anchor);
box encode_box_TA(box b, box anchor);

#endif
