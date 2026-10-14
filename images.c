#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char get_red(uint32_t pixel){
    pixel <<= 8;
    pixel >>= 24;
    return pixel;
}
unsigned char get_green(uint32_t pixel){
    pixel <<= 16;
    pixel >>= 24;
    return pixel;
}
unsigned char get_blue(uint32_t pixel){
    pixel &= 0x000000FF;
    return pixel;
}
unsigned char get_Alpha(uint32_t pixel){
    pixel &= 0xFF000000;
    pixel>>=24;
    return pixel;
}

uint32_t get_pix(unsigned char al, unsigned char r, unsigned char g, unsigned char b){
    uint32_t pix = al;
    pix <<= 8;
    pix |= r; 
    pix <<= 8;
    pix |= g;
    pix <<= 8;
    pix |= b;
    return pix;
}

uint32_t invert(uint32_t pix){
    return (pix^0x00FFFFFF);
}

struct image
{
    uint32_t width;
    uint32_t height;
    uint32_t * pixels;
};

struct image *create_image(uint32_t width, uint32_t height){
    struct image *img = malloc(sizeof(struct image));
    if (img == NULL){return NULL;};
    img->width = width;
    img->height = height;
    img->pixels = malloc(width*height*sizeof(uint32_t));
    return img;
}
void destroy_image(struct image *img){
    if(img==NULL){return;};
    free(img->pixels);
    free(img);
}

int save_image(const char *filename, const struct image *img){
    if(img==NULL){return-1;};
    FILE * f = fopen(filename, "wb");
    if(f==NULL){return -1;};
    fwrite(&(img->width), sizeof(img->width),1, f);
    fwrite(&(img->height), sizeof(img->height),1, f);
    fwrite(img->pixels,sizeof(*img->pixels),img->width*img->height,f);
    return (fclose(f)==0)-1;
}

struct bmp_info {
    uint32_t pixel_offset;
    int32_t width;
    int32_t height;
    uint16_t bits_per_pixel;
    uint32_t compression;
};

// 0 no,  1 yes
int is_bmp(const char *filename){
    FILE *f = fopen(filename, "rb");
    if(f==NULL){return 0;};
    char temp[2];
    fread(&temp, sizeof(char), 2, f);
    fclose(f);
    if(temp[0]=='B' && temp[1] == 'M'){return 1;}
    return 0;
}

struct image *load_image(const char *filename){
    FILE * f = fopen(filename, "rb");
    if(f==NULL){return NULL;};
    uint32_t height;
    uint32_t width;
    if(fread(&width, sizeof(uint32_t), 1, f)!= 1){fclose(f);return NULL;};
    if(fread(&height, sizeof(uint32_t), 1, f)!= 1){fclose(f);return NULL;};
    struct image* img = create_image(width, height);
    if(img==NULL){fclose(f);return NULL;};
    if(fread(img->pixels, sizeof(uint32_t), height*width, f) != width*height){fclose(f);destroy_image(img); return NULL;};
    fclose(f);
    return img;
}

void invert_image(struct image *img){
    if(img==NULL){return;};
    uint32_t *end = img->pixels+img->width*img->height;
    for(uint32_t *ptr = img->pixels; ptr < end; ptr++){
        if(ptr == NULL){return;};
        *ptr = invert(*ptr);
    }
}

void set_pixel(struct image* img, uint32_t x,uint32_t y, uint32_t color){
    if(x >= img->width || y >= img->height){return;};
    uint32_t * pix = img->pixels + (img->width * y)+x;
    *pix = color;
}

uint32_t get_pixel(struct image *img, uint32_t x, uint32_t y){
    if(x >= img->width || y >= img->height){return 0;};
    uint32_t * pix = img->pixels + (img->width * y)+x;
    return *pix;
}

//0 - al, r -1 etc..
uint32_t set_channel(uint32_t pix, uint8_t val, uint8_t mode){
    pix &= ((0x000000FF << (3-mode) * 8) ^ 0xFFFFFFFF);
    pix |= ((uint32_t)val << 8*(3-mode));
    return pix;
}

void print_pix(uint32_t pix){
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", get_red(pix), get_green(pix), get_blue(pix));
}
void print_img(struct image *img){
    for(uint32_t row = 0; row < img->height; row++){
        for(uint32_t col = 0; col < img->width; col++){
            print_pix(*(img->pixels+(row*img->width + col)));
        }
        printf("\n");
    }
}


int main(){
    uint32_t pixel = 0xFF3366CC;

    pixel = set_channel(pixel, 0x77, 2);
    printf("Alpha: %d, Red: %d, G: %d, B: %d\n", get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel));
    printf("Color: 0x%08X\n", get_pix(get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel)));

    printf("Inverted pixel:  0x%08X\n", invert(pixel));

    struct image * img = load_image("test_image");
    print_img(img);
    invert_image(img);
    print_img(img);

    return 0;
}