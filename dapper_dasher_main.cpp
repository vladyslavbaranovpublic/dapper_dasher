#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    int frame_y;
};

struct MovingBackData
{
    Texture2D texture;
    float x;
    Vector2 pos;
};

MovingBackData updateBackData(MovingBackData data, int speed, float deltaTime, int i, int windowDimension, int offset, int backgroundSizeOffset)
{
    data.x -= 32*speed * deltaTime;
    data.pos={data.x, 0.0};
    /*if(i==0){
        data.pos={data.x, 0.0};
    }
    else{
        data.pos={data.x + windowDimension*i, 0.0}; 
    }*/
      

    if(data.x <= -offset)
    {
        data.x = offset;
    }
    return data;
}
MovingBackData initilizeSlidingImage(MovingBackData data, Texture2D texture, int offset, int i)
{
    data.texture = texture;
    data.x = 0.0 + offset*i;
    data.pos.x = 0.0 + offset*i;
    data.pos.y = 0.0;
    return data;
}

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}
AnimData updateAnimData(AnimData data, int deltaTime, int maxFrame_x, bool isXOnlyTexture, int maxFrame_y, int maxFrame_xend)
{
    //data.runningTime += deltaTime;
    if(isXOnlyTexture == true){
        if(data.runningTime >= data.updateTime){
            data.runningTime = 0.0;
            data.rec.x = data.frame * data.rec.width;
            data.frame++;
            if(data.frame > maxFrame_x){
                data.frame = 0;
            }
        }
    }
    else{
        if(data.runningTime >= data.updateTime){
            data.rec.x = data.frame * data.rec.width;
            data.rec.y = data.frame_y * data.rec.height;
            data.runningTime = 0.0;
            data.frame++;
            if(data.frame > maxFrame_x){
                data.frame = 0;
                data.frame_y++;
            }
            if(data.frame_y > maxFrame_y && data.frame > maxFrame_xend){
                data.frame_y = 0;
                data.frame = 0;
            }
        }
    }

    return data;
}
int main(){
    const float windowDimension[2] = {512,380};
    InitWindow(windowDimension[0], windowDimension[1], "Dapper Dasher by Vlad Baranov");

    //air check
    bool isInAir = false;

    //physics
    int gravity{1'000};
    const int jump_force{600};
    int velocity{-10};

    //scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimension[0]/2-scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.frame_y = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;


    //nubula hazard
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};

    AnimData nubulae[sizeOfNebulae]{};

    for(int i = 0; i < sizeOfNebulae; i++)
    {
        nubulae[i].rec.x = 0.0;
        nubulae[i].rec.y = 0.0;
        nubulae[i].rec.width = nebula.width/8;
        nubulae[i].rec.height = nebula.height/8;
        nubulae[i].pos.y = windowDimension[1] - (float)nebula.height/8;
        nubulae[i].frame = 0;
        nubulae[i].runningTime = 0.0;
        nubulae[i].updateTime = 1.0/16.0;
        nubulae[i].frame_y = 0;

        nubulae[i].pos.x =  windowDimension[0] + i * 300;
    }
    float finishLine( nubulae[sizeOfNebulae-1].pos.x);

    for(int i=0; i < sizeOfNebulae; i++)
        {
            nubulae[i].pos.x = windowDimension[0] + 300*i;
        }

    //nubula velocity pixels/second
    int nebVel{-250};

    

    //background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    
    MovingBackData backgroundData[2]{};
    MovingBackData midgroundData[2]{};
    MovingBackData foregroundData[2]{};

    for(int i = 0; i < 2; i++)
    {
        backgroundData[i]   = initilizeSlidingImage(backgroundData[i], background,background.width*2, i);
        midgroundData[i]    = initilizeSlidingImage(midgroundData[i], midground, midground.width*2, i);
        foregroundData[i]   = initilizeSlidingImage(foregroundData[i], foreground, foreground.width*2, i);
        /*backgroundData[i].pos.x = windowDimension[0] * i;
        midgroundData[i].pos.x = windowDimension[0] * i;
        foregroundData[i].pos.x = windowDimension[0] * i;*/
    }
    bool collision{false};
    SetTargetFPS(120);

    while(!WindowShouldClose()){
        const float dT{GetFrameTime()};
        

        BeginDrawing();
        ClearBackground(WHITE);
        
        //display background
        for(int i = 0; i < 2; i++)
        {
            //update background, sends a call to update background data and within the call draws the background
            backgroundData[i] = updateBackData(backgroundData[i], 1, dT, i, windowDimension[0], 512, 0);
            DrawTextureEx(backgroundData[i].texture, backgroundData[i].pos, 0.0, 2.0, WHITE);
        }
        for(int i = 0; i < 2; i++)
        {
            midgroundData[i] = updateBackData(midgroundData[i], 2, dT, i, windowDimension[0], 512, 0);
            DrawTextureEx(midgroundData[i].texture, midgroundData[i].pos, 0.0, 2.0, WHITE);
        }
        for(int i = 0; i < 2; i++)
        {
           foregroundData[i] = updateBackData(foregroundData[i], 4, dT, i, windowDimension[0], 704, 0);
           DrawTextureEx(foregroundData[i].texture, foregroundData[i].pos, 0.0, 2.0, WHITE);
        }
        if(isOnGround(scarfyData, windowDimension[1]))
        {
            isInAir = false;
            velocity = 0;
        }
        else
        {
            isInAir = true;
            velocity += gravity * dT;
        }
        

        //up date velocity and jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity -= jump_force;
        }





        finishLine += nebVel * dT;


        //update Y position of character
        scarfyData.pos.y += velocity * dT;

        //animation update scarfyFrame
        if(isOnGround(scarfyData, windowDimension[1]))
        {
            scarfyData = updateAnimData(scarfyData, dT, 5, true, 0, 5);
        }

        //update running time
        scarfyData.runningTime += dT;

        for(AnimData& nubula : nubulae)
        {
            float pad{20.0};
            Rectangle nebRec{
                nubula.pos.x + pad,
                nubula.pos.y + pad,
                nubula.rec.width - pad*2,
                nubula.rec.height - pad*2
            };
            Rectangle scarfyRec{
                scarfyData.pos.x + pad,
                scarfyData.pos.y + pad,
                scarfyData.rec.width - pad*2,
                scarfyData.rec.height - pad*2
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
                
            }
        }
        if(collision)
        {
            DrawText("Game Over", 200, 200, 20, RED);
        }
        else if(scarfyData.pos.x >= finishLine){
            DrawText("You Win", 200, 200, 20, RED);
        }
        else{
            for(int i=0; i < sizeOfNebulae; i++){
            
                if (nubulae[i].pos.x + nubulae[i].rec.width >= 0 && nubulae[i].pos.x <= windowDimension[0]) {
                    nubulae[i].pos.x += nebVel * dT;

                //nebula animation update 1
                    nubulae[i].runningTime += dT; //for some reason without this it does not work not sure why
                    nubulae[i] = updateAnimData(nubulae[i], dT, 7, false, 6, 3);
                    

                    DrawTextureRec(nebula, nubulae[i].rec, nubulae[i].pos, WHITE);

                }
                else{nubulae[i].pos.x += nebVel * dT;}
            
            }


            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }




        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
