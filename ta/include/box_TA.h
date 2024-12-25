#ifndef BOX_TA_H
#define BOX_TA_H
#include "darknet_TA.h"

typedef struct{
    float dx, dy, dw, dh;
} dbox_TA;

float box_rmse_TA(box_TA a, box_TA b);
box_TA float_to_box_TA(float *f, int stride);
float box_iou_TA(box_TA a, box_TA b);
dbox_TA diou_TA(box_TA a, box_TA b);
box_TA decode_box_TA(box_TA b, box_TA anchor);
box_TA encode_box_TA(box_TA b, box_TA anchor);

#endif
