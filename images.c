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




int main(){
    uint32_t pixel = 0xFF3366CC;

    pixel = set_channel(pixel, 0x77, 2);
    printf("Alpha: %d, Red: %d, G: %d, B: %d\n", get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel));
    printf("Color: 0x%08X\n", get_pix(get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel)));

    printf("Inverted pixel:  0x%08X\n", invert(pixel));

    return 0;
}