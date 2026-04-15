#define COL_SYMBOLS 92
char col_scale[COL_SYMBOLS] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
float col_scale_weight[COL_SYMBOLS] = {0, 0.0751, 0.0829, 0.0848, 0.1227, 0.1403, 0.1559, 0.185, 0.2183, 0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099, 0.3192, 0.3232, 0.3294, 0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396, 0.3984, 0.3993, 0.4075, 0.4091, 0.4101, 0.42, 0.423, 0.4247, 0.4274, 0.4293, 0.4328, 0.4382, 0.4385, 0.442, 0.4473, 0.4477, 0.4503, 0.4562, 0.458, 0.461, 0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881, 0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569, 0.5591, 0.5602, 0.5602, 0.565, 0.5776, 0.5777, 0.5818, 0.587, 0.5972, 0.5999, 0.6043, 0.6049, 0.6093, 0.6099, 0.6465, 0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809, 0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834, 0.8037, 0.9999};

#define COL_COL 30
#define COL_LIGHT 90
#define COL_jump 10

//no col change
char rn[1] = "F";
//fg or bg change
char rc[6] = "\033[00mF";
//bg change >100
char rbh[7] = "\033[000mF";
//both
char rt[9] = "\033[00;00mF";
//both >100
char rth[10] = "\033[000;00mF";

int cols[16][3]={
                    {0,0,0},
                    {197,15,31},
                    {19,161,14},
                    {193,156,0},
                    {0,55,218},
                    {136,23,152},
                    {58,150,221},
                    {204,204,204},
                    {118,118,118},
                    {231,72,86},
                    {22,198,12},
                    {249,241,165},
                    {59,120,255},
                    {180,0,158},
                    {97,214,214},
                    {242,242,242},
                };

char *col2ascii(int r, int g, int b, char *bg, char *fg, int *len){
    char c1=-1;
    char c2=-1;
    float clensq=10000000000000000000.0;
    float mix;

    for (int i=0; i<16; i++){
        for (int j=i+1; j<16; j++){
            //proyect color onto line
            float vx = r-cols[i][0];
            float vy = g-cols[i][1];
            float vz = b-cols[i][2];
            float dx = cols[j][0]-cols[i][0];
            float dy = cols[j][1]-cols[i][1];
            float dz = cols[j][2]-cols[i][2];
            float m = (vx*dx+vy*dy+vz*dz)/(dx*dx+dy*dy+dz*dz);

            //clamp
            if (m<0) m=0;
            if (m>1) m=1;

            float dr = (cols[i][0]*(1-m)+cols[j][0]*m)-r;
            float dg = (cols[i][1]*(1-m)+cols[j][1]*m)-g;
            float db = (cols[i][2]*(1-m)+cols[j][2]*m)-b;
            float lenghtsq=(dr*dr+dg*dg+db*db);

            //compare to best
            if (lenghtsq<=clensq){
                clensq = lenghtsq;

                if (m<=0.5){
                    c1 = i;
                    c2 = j;
                    mix = m*2;
                }else{
                    c1 = j;
                    c2 = i;
                    mix = 2-m*2;
                }
            }
        }
    }

    c1+=((c1<8)?COL_COL:COL_LIGHT-8)+COL_jump;
    c2+=(c2<8)?COL_COL:COL_LIGHT-8;

    //bin search to get character
    char chr;
    if (mix>col_scale_weight[1]){
        unsigned char le=0;
        unsigned char ri=COL_SYMBOLS-1;
        while (le<ri)
        {
            unsigned char m = (le+ri)/2;

            if (col_scale_weight[m]>mix){
                ri=m-1;
            }else{
                le=m+1;
            }
        }
        chr = col_scale[le];
    }else chr=col_scale[0];
    

    //printf("\033[%d;%dm%c\033[m %f",c1+COL_jump,c2,chr,mix);
    
    //make output
    char *res;
    if (*bg==c1){ //no bg change
        if (*fg==c2){ //no fg change
            res = rn;
            *len = 1;
            res[0]=chr;
        }else{ //fg change
            res = rc;
            *len = 6;
            res[2]=(char)((c2%100)/10)+48;
            res[3]=(char)(c2%10)+48;

            res[5]=chr;
        }
    }else{ //bg change
        if (*fg==c2){ //no fg change
            if (c1>=100){
                res = rbh;
                *len = 7;
                res[2]=(char)(c1/100)+48;
                res[3]=(char)((c1%100)/10)+48;
                res[4]=(char)(c1%10)+48;

                res[6]=chr;
            }else{
                res = rc;
                *len = 6;
                res[2]=(char)((c1%100)/10)+48;
                res[3]=(char)(c1%10)+48;

                res[5]=chr;
            }
        }else{ //fg change
            if (c1>=100){
                res = rth;
                *len = 10;
                res[2]=(char)(c1/100)+48;
                res[3]=(char)((c1%100)/10)+48;
                res[4]=(char)(c1%10)+48;

                res[6]=(char)(c2/10)+48;
                res[7]=(char)(c2%10)+48;

                res[9]=chr;
            }else{
                res = rt;
                *len = 9;
                res[2]=(char)((c1%100)/10)+48;
                res[3]=(char)(c1%10)+48;

                res[5]=(char)(c2/10)+48;
                res[6]=(char)(c2%10)+48;

                res[8]=chr;
            }
        }
    }

    *bg=c1;
    *fg=c2;
    
    return res;
}