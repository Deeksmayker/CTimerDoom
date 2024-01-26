struct {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
} typedef Entity;

struct {
    Entity left_dude, right_dude;
    Entity ball;
    float additional_position;
} typedef Pong;

Pong* init_pong(){
    Pong *new_pong = malloc(3 * sizeof(Entity));
    new_pong->left_dude.position  = (Vector2){50, render_buffer.height/2};
    new_pong->left_dude.size      = (Vector2){50, render_buffer.height/4};
    new_pong->left_dude.velocity  = (Vector2){0, 0};
    
    new_pong->right_dude.position = (Vector2){render_buffer.width-50, render_buffer.height/2};
    new_pong->right_dude.size     = (Vector2){50, render_buffer.height/4};
    new_pong->right_dude.velocity = (Vector2){0, 0};
    
    new_pong->ball.position       = (Vector2){render_buffer.width/2, render_buffer.height/2};
    new_pong->ball.size           = (Vector2){50, 50};
    new_pong->ball.velocity       = (Vector2){500, 500};
    
    new_pong->additional_position = 0;

    return new_pong;
}

void calculate_collisions(Entity **entities){
    Vector2 wish_position = {entities[0]->position.x + entities[0]->velocity.x * delta_time,
                             entities[0]->position.y + entities[0]->velocity.y * delta_time};
    if (wish_position.x < 0 || wish_position.x >= render_buffer.width - entities[0]->size.x){
        entities[0]->velocity.x *= -1; 
    }
    if (wish_position.y < 0 || wish_position.y >= render_buffer.height - entities[0]->size.y){
        entities[0]->velocity.y *= -1; 
    }
    
    int overlap_with_left_dude = entities[0]->velocity.x < 0
                              && wish_position.x >= entities[1]->position.x
                              && wish_position.x <= entities[1]->position.x + entities[1]->size.x
                              && wish_position.y >= entities[1]->position.y
                              && wish_position.y <= entities[1]->position.y + entities[1]->size.y;
                              
    int overlap_with_right_dude = entities[0]->velocity.x > 0
                              && wish_position.x + entities[0]->size.x >= entities[2]->position.x
                              && wish_position.x <= entities[2]->position.x + entities[2]->size.x
                              && wish_position.y >= entities[2]->position.y
                              && wish_position.y <= entities[2]->position.y + entities[2]->size.y;
    
    if (overlap_with_left_dude || overlap_with_right_dude){
        entities[0]->velocity.x *= -1;
        entities[0]->velocity.y += entities[1]->velocity.y;
    }
}

void update_pong(Pong *game){
    float move_speed = 300;
    float ball_speed = 500;
    //game->additional_position += input.vertical * delta_time * move_speed;
    game->left_dude.velocity.y  = input.vertical * move_speed;
    game->right_dude.velocity.y = input.vertical * move_speed;
    game->left_dude.position.y  += game->left_dude.velocity.y  * delta_time;
    game->right_dude.position.y += game->right_dude.velocity.y * delta_time;
    
    clamp(&game->left_dude.position.y, 0, render_buffer.height - game->left_dude.size.y);
    clamp(&game->right_dude.position.y, 0, render_buffer.height - game->right_dude.size.y);
    
    Entity **collision_entities = malloc(3 * sizeof(int));
    collision_entities[0] = &game->ball;
    collision_entities[1] = &game->left_dude;
    collision_entities[2] = &game->right_dude;
    calculate_collisions(collision_entities);
    game->ball.position.x += game->ball.velocity.x * delta_time;
    game->ball.position.y += game->ball.velocity.y * delta_time;

    
    //draw    
    game->right_dude.position.x = render_buffer.width-100;

    
    draw_rectF(game->left_dude.position,  game->left_dude.size, 0xffffff);
    draw_rectF(game->right_dude.position, game->right_dude.size, 0xff6666);
    draw_rectF(game->ball.position, game->ball.size, 0x6666ff);
}
