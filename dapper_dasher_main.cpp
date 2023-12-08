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
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
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
    }

    for(int i=0; i < sizeOfNebulae; i++)
        {
            nubulae[i].pos.x = windowDimension[0] + 300*i;
        }

    //nubula velocity pixels/second
    int nebVel{-90};


    SetTargetFPS(60);

    while(!WindowShouldClose()){
        const float dT{GetFrameTime()};
        

        BeginDrawing();
        ClearBackground(WHITE);
        
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



        for(int i=0; i < sizeOfNebulae; i++)
        {
            
            if (nubulae[i].pos.x + nubulae[i].rec.width >= 0 && nubulae[i].pos.x <= windowDimension[0]) {
                nubulae[i].pos.x += nebVel * dT;
            
            //nebula animation update 1
                nubulae[i].runningTime += dT;
                if(nubulae[i].runningTime >= nubulae[i].updateTime){
                    nubulae[i].rec.x = nubulae[i].frame*nubulae[i].rec.width;
                    nubulae[i].rec.y = nubulae[i].frame_y*nubulae[i].rec.height;
                    nubulae[i].runningTime = 0.0;
                    nubulae[i].frame++;
                    if(nubulae[i].frame > 7){
                        nubulae[i].frame = 0;
                        nubulae[i].frame_y++;
                    }
                    if(nubulae[i].frame_y > 6 && nubulae[i].frame > 3)
                    {
                        nubulae[i].frame_y = 0;
                        nubulae[i].frame = 0;
                    }
                }
                DrawTextureRec(nebula, nubulae[i].rec, nubulae[i].pos, WHITE);
            }
            else{nubulae[i].pos.x += nebVel * dT;}
            
        }


        //update Y position of character
        scarfyData.pos.y += velocity * dT;

        //animation update scarfyFrame
        if(!isInAir)
        {
            scarfyData.rec.x = scarfyData.frame*scarfyData.rec.width;
            if(scarfyData.runningTime >= scarfyData.updateTime){
            scarfyData.runningTime = 0.0;
            scarfyData.frame++;
                if(scarfyData.frame > 5){
                    
                    scarfyData.frame = 0;
                }

            }
        }

        //update running time
        scarfyData.runningTime += dT;

        // draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);


        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}
