#include<ctype.h>
#include "my_math.c"

// #define LETTER_PIXEL_SIZE 10;
#define LETTER_WIDTH 4;
#define LETTER_HEIGHT 7;
//#define HW_RATIO (8f/5f);

void clear_screen(u32 color){
    //u32 *pixels = render_buffer.pixels;
    for (int y = 0; y < render_buffer.height * render_buffer.width; y++){
        render_buffer.pixels[y] = color;
    }
}

void clear_screen_gradient(u32 *colors, int colors_count){
    for (int y = 0; y < render_buffer.height; y++){
        u32 row_color = lerp_colors(colors, colors_count, ((float)y) / ((float)render_buffer.height));
        for (int x = 0; x < render_buffer.width; x++){
            render_buffer.pixels[y * render_buffer.width + x] = row_color;
        }
    }
}

void draw_rect(Vector2_int position, Vector2_int size, u32 color){
    position.x %= render_buffer.width;
    position.y %= render_buffer.height;

    clamp_int(&position.x, 0, render_buffer.width  - size.x - 1);
    clamp_int(&position.y, 0, render_buffer.height - size.x - 1);

    for (int y = position.y; y < position.y + size.y && y < render_buffer.height; y++){
        //u32  *pixel = render_buffer.pixels + x0 + render_buffer.width*y;
        int y_index = position.x + render_buffer.width * y;
        for (int x = 0; x < size.x && x < render_buffer.width; x++){
            render_buffer.pixels[y_index + x] = color;
        }
    }
}

void draw_rectF(Vector2 position, Vector2 size, u32 color){
    Vector2_int int_position = {(int)position.x, (int)position.y};
    Vector2_int int_size = {(int)size.x, (int)size.y};
    draw_rect(int_position, int_size, color);
}


void draw_digit(int digit, Vector2_int position, int pixel_size, u32 color){
    digit %= 10;
    int width = pixel_size * LETTER_WIDTH;
    int height = pixel_size * LETTER_HEIGHT;

    // position.x = clamp_int(position.x, 0, render_buffer.width);
    // position.y = clamp_int(position.y, 0, render_buffer.height-1);

    Vector2_int left_up_pos = {position.x, position.y+height-pixel_size};
    Vector2_int right_down_pos = {position.x+width-pixel_size, position.y};
    Vector2_int left_middle_pos = {position.x, position.y + height/2};
    Vector2_int right_middle_pos = {position.x+width-pixel_size, position.y + height/2};

    if (digit == 0){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(position, (Vector2_int){pixel_size, height}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
    } else if (digit == 1){
        draw_rect((Vector2_int){position.x + width/2, position.y+height-pixel_size}, (Vector2_int){width/2, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
    } else if (digit == 2){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(position, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_middle_pos, (Vector2_int){pixel_size, height/2}, color); 
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
    } else if (digit == 3){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_middle_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
    } else if (digit == 4){
        draw_rect(left_middle_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
    } else if (digit == 5){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(left_middle_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
    } else if (digit == 6){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(position, (Vector2_int){pixel_size, height}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
    } else if (digit == 7){
        draw_rect((Vector2_int){position.x, position.y+height-pixel_size}, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
    } else if (digit == 8){
        draw_rect(position, (Vector2_int){width, pixel_size}, color);
        draw_rect(position, (Vector2_int){pixel_size, height}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
    } else if (digit == 9){
        draw_rect(left_middle_pos, (Vector2_int){pixel_size, height/2}, color);
        draw_rect(left_up_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(left_middle_pos, (Vector2_int){width, pixel_size}, color);
        draw_rect(right_down_pos, (Vector2_int){pixel_size, height}, color);
    }
}

void draw_letter(char letter, Vector2_int position, int pixel_size, u32 color){
    int width = pixel_size * LETTER_WIDTH;
    int height = pixel_size * LETTER_HEIGHT;

    if (letter == '.'){
        draw_rect((Vector2_int){position.x + width/2, position.y},(Vector2_int){pixel_size, pixel_size}, color); 
    } else if (letter == ':'){
        draw_rect((Vector2_int){position.x + width/2, position.y},(Vector2_int){pixel_size, pixel_size}, color); 
        draw_rect((Vector2_int){position.x + width/2, position.y+height-pixel_size},(Vector2_int){pixel_size, pixel_size}, color); 
    }
}

void draw_text(char *text, Vector2_int position, int pixel_size, u32 color){
    int offset = 0;
    int width = LETTER_WIDTH;
    int len = strlen(text);
    for (int i = 0; i < len; i++){
        if (isdigit(text[i])){
            draw_digit((int)text[i] - '0', (Vector2_int){position.x + offset, position.y}, pixel_size, color);
        } else{
            draw_letter(text[i], (Vector2_int){position.x + offset, position.y}, pixel_size, color);
        }
        offset += (pixel_size * width) + pixel_size;
    }
}

void draw_time(int hours, int minutes, int seconds, Vector2_int position, int pixel_size, u32 color){
    int offset = 0;
    int width = LETTER_WIDTH;
    
    draw_digit((hours/10) , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit((hours%10) , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_letter(':', (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    
    draw_digit(minutes/10 , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit(minutes%10 , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_letter(':', (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;

    draw_digit(seconds/10 , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
    draw_digit(seconds%10 , (Vector2_int){position.x + offset, position.y}, pixel_size, color);
    offset += (pixel_size * width) + pixel_size;
}
