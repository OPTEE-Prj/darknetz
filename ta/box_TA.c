#include "box_TA.h"
#include <stdio.h>
#include "math_TA.h"
#include <stdlib.h>

int nms_comparator_TA(const void *pa, const void *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    float diff = 0;
    if(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } else {
        diff = a.objectness - b.objectness;
    }
    if(diff < 0) return 1;
    else if(diff > 0) return -1;
    return 0;
}

void do_nms_TA_obj_TA(detection *dets, int total, int classes, float thresh)
{
    int i, j, k;
    k = total-1;
    for(i = 0; i <= k; ++i){
        if(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    for(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, sizeof(detection), nms_comparator_TA);
    for(i = 0; i < total; ++i){
        if(dets[i].objectness == 0) continue;
        box_TA a = dets[i].bbox;
        for(j = i+1; j < total; ++j){
            if(dets[j].objectness == 0) continue;
            box_TA b = dets[j].bbox;
            if (box_iou_TA(a, b) > thresh){
                dets[j].objectness = 0;
                for(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


void do_nms_TA_sort_TA(detection *dets, int total, int classes, float thresh)
{
    int i, j, k;
    k = total-1;
    for(i = 0; i <= k; ++i){
        if(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    for(k = 0; k < classes; ++k){
        for(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, sizeof(detection), nms_comparator_TA);
        for(i = 0; i < total; ++i){
            if(dets[i].prob[k] == 0) continue;
            box_TA a = dets[i].bbox;
            for(j = i+1; j < total; ++j){
                box_TA b = dets[j].bbox;
                if (box_iou_TA(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box_TA float_to_box_TA(float *f, int stride)
{
    box_TA b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    return b;
}

dbox_TA derivative_TA(box_TA a, box_TA b)
{
    dbox_TA d;
    d.dx = 0;
    d.dw = 0;
    float l1 = a.x - a.w/2;
    float l2 = b.x - b.w/2;
    if (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    float r1 = a.x + a.w/2;
    float r2 = b.x + b.w/2;
    if(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    if (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    if (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    float t1 = a.y - a.h/2;
    float t2 = b.y - b.h/2;
    if (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    float b1 = a.y + a.h/2;
    float b2 = b.y + b.h/2;
    if(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    if (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    if (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    return d;
}

float overlap_TA(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1/2;
    float l2 = x2 - w2/2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1/2;
    float r2 = x2 + w2/2;
    float right = r1 < r2 ? r1 : r2;
    return right - left;
}

float box_intersection_TA(box_TA a, box_TA b)
{
    float w = overlap_TA(a.x, a.w, b.x, b.w);
    float h = overlap_TA(a.y, a.h, b.y, b.h);
    if(w < 0 || h < 0) return 0;
    float area = w*h;
    return area;
}

float box_union_TA(box_TA a, box_TA b)
{
    float i = box_intersection_TA(a, b);
    float u = a.w*a.h + b.w*b.h - i;
    return u;
}

float box_iou_TA(box_TA a, box_TA b)
{
    return box_intersection_TA(a, b)/box_union_TA(a, b);
}

float box_rmse_TA(box_TA a, box_TA b)
{
    return ta_sqrt(ta_pow(a.x-b.x, 2) + 
                ta_pow(a.y-b.y, 2) + 
                ta_pow(a.w-b.w, 2) + 
                ta_pow(a.h-b.h, 2));
}

dbox_TA dintersect_TA(box_TA a, box_TA b)
{
    float w = overlap_TA(a.x, a.w, b.x, b.w);
    float h = overlap_TA(a.y, a.h, b.y, b.h);
    dbox_TA dover = derivative_TA(a, b);
    dbox_TA di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    return di;
}

dbox_TA dunion_TA(box_TA a, box_TA b)
{
    dbox_TA du;

    dbox_TA di = dintersect_TA(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    return du;
}


void test_dunion_TA()
{
    box_TA a = {0, 0, 1, 1};
    box_TA dxa= {0+.0001, 0, 1, 1};
    box_TA dya= {0, 0+.0001, 1, 1};
    box_TA dwa= {0, 0, 1+.0001, 1};
    box_TA dha= {0, 0, 1, 1+.0001};

    box_TA b = {.5, .5, .2, .2};
    dbox_TA di = dunion_TA(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    float inter =  box_union_TA(a, b);
    float xinter = box_union_TA(dxa, b);
    float yinter = box_union_TA(dya, b);
    float winter = box_union_TA(dwa, b);
    float hinter = box_union_TA(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
void test_dintersect_TA()
{
    box_TA a = {0, 0, 1, 1};
    box_TA dxa= {0+.0001, 0, 1, 1};
    box_TA dya= {0, 0+.0001, 1, 1};
    box_TA dwa= {0, 0, 1+.0001, 1};
    box_TA dha= {0, 0, 1, 1+.0001};

    box_TA b = {.5, .5, .2, .2};
    dbox_TA di = dintersect_TA(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    float inter =  box_intersection_TA(a, b);
    float xinter = box_intersection_TA(dxa, b);
    float yinter = box_intersection_TA(dya, b);
    float winter = box_intersection_TA(dwa, b);
    float hinter = box_intersection_TA(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

void test_box_TA()
{
    test_dintersect_TA();
    test_dunion_TA();
    box_TA a = {0, 0, 1, 1};
    box_TA dxa= {0+.00001, 0, 1, 1};
    box_TA dya= {0, 0+.00001, 1, 1};
    box_TA dwa= {0, 0, 1+.00001, 1};
    box_TA dha= {0, 0, 1, 1+.00001};

    box_TA b = {.5, 0, .2, .2};

    float iou = box_iou_TA(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox_TA d = diou_TA(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    float xiou = box_iou_TA(dxa, b);
    float yiou = box_iou_TA(dya, b);
    float wiou = box_iou_TA(dwa, b);
    float hiou = box_iou_TA(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox_TA diou_TA(box_TA a, box_TA b)
{
    float u = box_union_TA(a,b);
    float i = box_intersection_TA(a,b);
    dbox_TA di = dintersect_TA(a,b);
    dbox_TA du = dunion_TA(a,b);
    dbox_TA dd = {0,0,0,0};

    if(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        return dd;
    }

    dd.dx = 2*ta_pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*ta_pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*ta_pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*ta_pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    return dd;
}


void do_nms_TA(box_TA *boxes, float **probs, int total, int classes, float thresh)
{
    int i, j, k;
    for(i = 0; i < total; ++i){
        int any = 0;
        for(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        if(!any) {
            continue;
        }
        for(j = i+1; j < total; ++j){
            if (box_iou_TA(boxes[i], boxes[j]) > thresh){
                for(k = 0; k < classes; ++k){
                    if (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    else probs[j][k] = 0;
                }
            }
        }
    }
}

box_TA encode_box_TA(box_TA b, box_TA anchor)
{
    box_TA encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = ta_log(2, b.w / anchor.w);
    encode.h = ta_log(2, b.h / anchor.h);
    return encode;
}

box_TA decode_box_TA(box_TA b, box_TA anchor)
{
    box_TA decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = ta_pow(2., b.w) * anchor.w;
    decode.h = ta_pow(2., b.h) * anchor.h;
    return decode;
}
