#ifndef _EI_CLASSIFIER_TFLITE_RESOLVER_H_
#define _EI_CLASSIFIER_TFLITE_RESOLVER_H_

#include "edge-impulse-sdk/tensorflow/lite/micro/kernels/micro_ops.h"

#define EI_TFLITE_RESOLVER static tflite::MicroMutableOpResolver<7> resolver; \
    resolver.AddAdd(); \
    resolver.AddConv2D(); \
    resolver.AddDepthwiseConv2D(); \
    resolver.AddFullyConnected(); \
    resolver.AddReshape(); \
    resolver.AddSoftmax(); \
    resolver.AddPad();

#endif // _EI_CLASSIFIER_TFLITE_RESOLVER_H_
#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_PERSON_DETECT_MODEL_DATA_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_PERSON_DETECT_MODEL_DATA_H_

extern const unsigned char g_person_detect_model_data[];
extern const int g_person_detect_model_data_len;

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_PERSON_DETECT_MODEL_DATA_H_