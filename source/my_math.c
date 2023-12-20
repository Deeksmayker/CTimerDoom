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

int lerp_color(uint32_t color1, uint32_t color2, float fraction)
{
        unsigned char   r1 = (color1 >> 16) & 0xff;
        unsigned char   r2 = (color2 >> 16) & 0xff;
        unsigned char   g1 = (color1 >> 8) & 0xff;
        unsigned char   g2 = (color2 >> 8) & 0xff;
        unsigned char   b1 = color1 & 0xff;
        unsigned char   b2 = color2 & 0xff;

        return (int) ((r2 - r1) * fraction + r1) << 16 |
                (int) ((g2 - g1) * fraction + g1) << 8 |
                (int) ((b2 - b1) * fraction + b1);
}
