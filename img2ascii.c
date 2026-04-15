#include <stdio.h>
#include "col2ascii.c"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

int width;
int height;
int channels;
void GetCol(unsigned char* img, int x, int y, int *r, int *g, int*b){
    if (x<0) x=0;
    if (y<0) y=0;
    if (x>=width) x=width-1;
    if (y>=height) y=height-1;

    int index = (y * width + x) * channels;

    *r = img[index];
    *g = img[index+1];
    *b = img[index+2];
}

int main(){
    char source[128];
    int hh;
    scanf("%s %d", source, &hh);

    int r,g,b;
    unsigned char *img = stbi_load(source, &width, &height, &channels, 0);

    int ww = 19*width*hh/(9*height); //char_ratio(19/9)*aspect_ratio*height

    
   int xstep=width/ww;
   int ystep=height/hh;
   char bg=-1, fg=-1;
   for (int y=0; y<hh; y++){
        for (int x=0; x<ww; x++){
            unsigned rr=0;
            unsigned gg=0;
            unsigned bb=0;
            for (int i=0; i<xstep; i++){
                for (int j=0; j<ystep; j++){
                    GetCol(img,x*xstep+i,y*ystep+j,&r,&g,&b);
                    rr+=r;
                    gg+=g;
                    bb+=b;
                }
            }
            unsigned n=xstep*ystep;
            int len=0;
            char *s = col2ascii(rr/n,gg/n,bb/n, &bg,&fg,&len);
            fwrite(s, 1, len, stdout);
        }
        bg=-1; fg=-1;
        printf("\033[m\n");
    }

    printf("\033[m");
}