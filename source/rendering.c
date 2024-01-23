#include<ctype.h>
#include "my_math.c"

// #define LETTER_PIXEL_SIZE 10;
#define LETTER_WIDTH 4;
#define LETTER_HEIGHT 7;
//#define HW_RATIO (8f/5f);

void clear_screen(uint32_t color){
    //uint32_t *pixels = render_buffer.pixels;
    for (int y = 0; y < render_buffer.height * render_buffer.width; y++){
        render_buffer.pixels[y] = color;
    }

    // for (int y = 0; y < render_buffer.height; y++){
    //     for (int x = 0; x < render_buffer.width; x++){
    //         *pixels++ = color;
    //     }
    // }
}

void clear_screen_gradient(uint32_t color1, uint32_t color2){
    for (int y = 0; y < render_buffer.height; y++){
        uint32_t row_color = lerp_color(color1, color2, ((float)y) / ((float)render_buffer.height));
        for (int x = 0; x < render_buffer.width; x++){
            render_buffer.pixels[y * render_buffer.width + x] = row_color;
        }
    }

}

void clear_screen_three_color(uint32_t color1, uint32_t color2, uint32_t color3){
    for (int y = 0; y < render_buffer.height; y++){
        uint32_t ab = lerp_color(color1, color2, ((float)y) / ((float)render_buffer.height));
        uint32_t bc = lerp_color(color2, color3, ((float)y) / ((float)render_buffer.height));
        uint32_t row_color = lerp_color(ab, bc, ((float)y) / ((float)render_buffer.height));
        for (int x = 0; x < render_buffer.width; x++){
            render_buffer.pixels[y * render_buffer.width + x] = row_color;
        }
    }

}

void draw_rect(int_vector2 pos, int_vector2 size, uint32_t color){
    // x0 = clamp_int(x0, 0, x1);
    // y0 = clamp_int(y0, 0, y1);
    // x1 = clamp_int(x1, x0, render_buffer.width);
    // y1 = clamp_int(y1, y0, render_buffer.height);
    // pos.x = clamp_int(pos.x, 0, render_buffer.width - size.x - 1);
    // pos.y = clamp_int(pos.y, 0, render_buffer.height - size.x - 1);
    pos.x %= render_buffer.width;
    pos.y %= render_buffer.height;

    pos.x = clamp_int(pos.x, 0, render_buffer.width - size.x - 1);
    pos.y = clamp_int(pos.y, 0, render_buffer.height - size.x - 1);

    //if (pos.x == 0) pos.x += render_buffer.width - 1;

    for (int y = pos.y; y < pos.y + size.y && y < render_buffer.height; y++){
        //uint32_t  *pixel = render_buffer.pixels + x0 + render_buffer.width*y;
        int y_index = pos.x + render_buffer.width * y;
        for (int x = 0; x < size.x && x < render_buffer.width; x++){
            render_buffer.pixels[y_index + x] = color;
        }
    }
}

void draw_digit(int digit, int_vector2 pos, int pixel_size, uint32_t color){
    digit %= 10;
    int width = pixel_size * LETTER_WIDTH;
    int height = pixel_size * LETTER_HEIGHT;

    // pos.x = clamp_int(pos.x, 0, render_buffer.width);
    // pos.y = clamp_int(pos.y, 0, render_buffer.height-1);

    int_vector2 left_up_pos = {pos.x, pos.y+height-pixel_size};
    int_vector2 right_down_pos = {pos.x+width-pixel_size, pos.y};
    int_vector2 left_middle_pos = {pos.x, pos.y + height/2};
    int_vector2 right_middle_pos = {pos.x+width-pixel_size, pos.y + height/2};

    if (digit == 0){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(pos, (int_vector2){pixel_size, height}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
    } else if (digit == 1){
        draw_rect((int_vector2){pos.x + width/2, pos.y+height-pixel_size}, (int_vector2){width/2, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
    } else if (digit == 2){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_middle_pos, (int_vector2){pixel_size, height/2}, color); 
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
    } else if (digit == 3){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_middle_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
    } else if (digit == 4){
        draw_rect(left_middle_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
    } else if (digit == 5){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(left_middle_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
    } else if (digit == 6){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(pos, (int_vector2){pixel_size, height}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
    } else if (digit == 7){
        draw_rect((int_vector2){pos.x, pos.y+height-pixel_size}, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
    } else if (digit == 8){
        draw_rect(pos, (int_vector2){width, pixel_size}, color);
        draw_rect(pos, (int_vector2){pixel_size, height}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
    } else if (digit == 9){
        draw_rect(left_middle_pos, (int_vector2){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(left_middle_pos, (int_vector2){width, pixel_size}, color);
        draw_rect(right_down_pos, (int_vector2){pixel_size, height}, color);
    }
}

void draw_letter(char letter, int_vector2 pos, int pixel_size, uint32_t color){
    int width = pixel_size * LETTER_WIDTH;
    int height = pixel_size * LETTER_HEIGHT;

    if (letter == '.'){
        draw_rect((int_vector2){pos.x + width/2, pos.y},(int_vector2){pixel_size, pixel_size}, color); 
    } else if (letter == ':'){
        draw_rect((int_vector2){pos.x + width/2, pos.y},(int_vector2){pixel_size, pixel_size}, color); 
        draw_rect((int_vector2){pos.x + width/2, pos.y+height-pixel_size},(int_vector2){pixel_size, pixel_size}, color); 
    }
}

void draw_text(char *text, int_vector2 pos, int pixel_size, uint32_t color){
    int offset = 0;
    int width = LETTER_WIDTH;
    int len = strlen(text);
    for (int i = 0; i < len; i++){
        if (isdigit(text[i])){
            draw_digit((int)text[i] - '0', (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
        } else{
            draw_letter(text[i], (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
        }
        offset += (pixel_size * width) + pixel_size;
    }
}

void draw_time(int hours, int minutes, int seconds, int_vector2 pos, int pixel_size, uint32_t color){
    int offset = 0;
    int width = LETTER_WIDTH;
    
    draw_digit((hours/10) , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit((hours%10) , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_letter(':', (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    
    draw_digit(minutes/10 , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit(minutes%10 , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_letter(':', (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_digit(seconds/10 , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit(seconds%10 , (int_vector2){pos.x + offset, pos.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
}
