#include <stdint.h>
#include <stdio.h>

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

//0 - al, r -1 etc..
uint32_t set_argb(uint32_t pix, uint8_t val, uint8_t mode){
    pix &= ((0x000000FF << (3-mode) * 8) ^ 0xFFFFFFFF);
    pix |= ((uint32_t)val << 8*(3-mode));
    return pix;
}


int main(){
    uint32_t pixel = 0xFF3366CC;

    pixel = set_argb(pixel, 0x77, 2);
    printf("Alpha: %d, Red: %d, G: %d, B: %d\n", get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel));
    printf("Color: 0x%08X\n", get_pix(get_Alpha(pixel), get_red(pixel), get_green(pixel), get_blue(pixel)));

    printf("Inverted pixel:  0x%08X\n", invert(pixel));

    return 0;
}