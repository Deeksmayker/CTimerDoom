#pragma once

typedef struct{
    int x;
    int y;
} Vector2_int;

typedef struct{
    float x;
    float y;
} Vector2;

void clamp(float *value, float min, float max){
    if (*value >= max) *value = max;
    if (*value <= min) *value = min;
}

void clamp_int(int *value, int min, int max){
    if (*value >= max) *value = max;
    if (*value <= min) *value = min;
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

float pow(float value, int power){
    if (power == 0) return 1;
    
    if (power & 1) return value * pow(value, power >> 1) * pow (value, power >> 1);
    else           return pow(value, power >> 1) * pow(value, power >> 1);
}

int factorial(int value){
    if (value <= 1) return 1;
    return value * factorial(value-1);
}

int bezie(int *numbers, int n, float t){
    int result = 0x00;
    for (int i = 0; i < n - 1; i++){
        result += (factorial(n) / (factorial(n - i) * factorial(i))) // Число сочетаний n по i
                  * pow((float)1 - t, n - i)
                  * pow(t, i) * numbers[i];
    }
    
    result += pow(t, n) * numbers[n - 1];
    
    return result;
}

int bezie_colors(uint32_t *colors, int colors_count, float fraction)
{
    int *reds   = malloc(colors_count * sizeof(int));    
    int *greens = malloc(colors_count * sizeof(int));    
    int *blues  = malloc(colors_count * sizeof(int));    
    
    for (int i = 0; i < colors_count; i++){
        reds  [i] = (colors[i] >> 16) & 0xff;
        greens[i] = (colors[i] >> 8)  & 0xff;
        blues [i] =  colors[i]        & 0xff;
    }
    
    int result =  bezie(reds, colors_count, fraction)   << 16 |
                  bezie(greens, colors_count, fraction) << 8  |
                  bezie(blues, colors_count, fraction);
    free(reds);
    free(greens);
    free(blues);
    return result;
}

int lerp_colors(uint32_t *colors, int colors_count, float fraction){
    //clamp(&fraction, (float)0, (float)0.9999999);
    int index  = lerp_int(0, colors_count-1, fraction);
    int color1 = colors[index];
    int color2 = colors[index+1];
    
    float t = (fraction - ((float)index/(colors_count-1))) * (colors_count-1);
    
    return lerp_color(color1, color2, t);
}
