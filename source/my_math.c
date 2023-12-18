#pragma once

typedef struct{
    int x;
    int y;
} int_vector2;

int clamp_int(int value, int min, int max){
    if (value >= max) return max;
    if (value <= min) return min;
    return value;
}

float lerp(float a, float b, float t){
    return ((float)1-t) * a + b * t;
}

int lerp_int(int a, int b, float t){
    return ((float)1-t) * a + b * t;
}
