#ifndef YOLO_LAYER_TA_H
#define YOLO_LAYER_TA_H

#include "darknet_TA.h"
#include "network_TA.h"

#include <stdio.h>
#include <time.h>

layer_TA make_yolo_layer_TA(int batch, int w, int h, int n, int total, int *mask, int classes);
void forward_yolo_layer_TA(const layer_TA l, network_TA net);
void backward_yolo_layer_TA(const layer_TA l, network_TA net);
void resize_yolo_layer_TA(layer_TA *l, int w, int h);
int yolo_num_detections_TA(layer_TA l, float thresh);

#ifdef GPU
void forward_yolo_layer_gpu_TA(const layer_TA l, network_TA net);
void backward_yolo_layer_gpu_TA(layer_TA l, network_TA net);
#endif

#endif
