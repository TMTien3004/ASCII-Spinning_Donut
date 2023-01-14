#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

void display(const char *output, float &A, float &B){
    printf("\x1b[H");
    //1760 = 22 * 80, which is the buffer size.
    for (int i = 0; i < 1761; i++){
        putchar(i % 80 ? output[i] : 10); //putchar() returns the written character in 26 letters
        A += 0.00004;
        B += 0.00002;
    }
}

void render(float A, float B, int buffer_size, float *zBuffer, char *output, int screen_height, int screen_width){
    int outer_size = screen_width / 40;
    char * lum_chars = ".,-~:;=!*#$@";
    //characters corresponding to different amounts of brightness;
    memset(output, ' ',buffer_size);
    memset(zBuffer, 0, buffer_size * sizeof(float));

    //To fix the smoothness, fix the increment of theta and pi. The lower the increment is, the slower the donut will spin.
    for (float theta = 0; theta < 6.28; theta += 0.01){
        for (float phi = 0; phi < 6.28; phi += 0.01){
            float sinPhi = sin(phi), cosPhi = cos(phi);
            float sinTheta = sin(theta), cosTheta = cos(theta);
            float sinA = sin(A), cosA = cos(A);
            float sinB = sin(B), cosB = cos(B);
            float circle_X = cosTheta + outer_size;
            float circle_Y = sinPhi * circle_X * cosA - sinTheta * sinA;
            
            //Normalized z, in this respect, z is a line from the monitor straight to your face, normalized for consistent output
            float norm_z = 1 / (sinPhi * circle_X * sinA + sinTheta * cosA + 5);

            //Calcuating x and y
            int x = (screen_width / 2) + 30 * norm_z * (cosPhi * circle_X * cosB + circle_Y * sinB);
            int y = (screen_height / 2 + 1) + 15 * norm_z * (cosPhi * circle_X * sinB - circle_Y * cosB);

            // Varible to store rendered ASCII character in the buffer.
            // We are using a 1D array.
            int o = x + screen_width * y;

            //Gives cross sections of donut (Level Curves in multivariable calculus)
            int lum = 8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB);
            
            //Luminance
            if (screen_height > y && y > 0 && x > 0 && screen_width > x && norm_z > zBuffer[o]) {
                zBuffer[o] = norm_z;
                output[o] = lum_chars[lum > 0 ? lum : 0];
            }
        }   
    }
}

int main(){
    float A = 0, B = 0;
    int screen_height = 22;
    int screen_width = 80;
    int buffer_size = screen_height * screen_width;
    float zBuffer[buffer_size];
    char output[buffer_size];
    for(;;){
        render(A, B, buffer_size, zBuffer, output, screen_height, screen_width);
        display(output, A, B);
    }
    return 0;
}


