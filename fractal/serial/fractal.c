#include "svpng.inc"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define W 4096
#define H 4096

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
    int x;
    int y;
} Point;

static unsigned char img[W * H * 3];

/**
* Line drawing (bresenham_line) and png saving functionality taken from line library.
* This library can be accessed by https://github.com/miloyip/line
*/

void setpixel(char* image, int x, int y) 
{
    unsigned char* p = image + (y * W + x) * 3;
    p[0] = p[1] = p[2] = 0;
}

// Modified from  https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void bresenham_line(char *image, int x0, int y0, int x1, int y1) 
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (setpixel(image, x0, y0), x0 != x1 || y0 != y1) {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}

void lineTo(char *image, Point p1, Point p2)
{
    bresenham_line(image, p1.x, p1.y, p2.x, p2.y);
}

// Sierpinski Carpet Fractal
void sierpinskicarpet(char *image, int x, int y, int size) {
    if (size <= 1) {
        // When size becomes one, color a pixel.
        setpixel(image, x, y);
    } else {
        int newSize = size / 3;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // If its center skip and for others continue recursivly
                if (!(i == 1 && j == 1)) {
                    if (x + i * newSize >= 0 && x + i * newSize < W &&
                        y + j * newSize >= 0 && y + j * newSize < H) {
                        sierpinskicarpet(image, x + i * newSize, y + j * newSize, newSize);
                    }
                }
            }
        }
    }
}

int main()
{
    memset(img, 255, sizeof(img));
    
    clock_t start, end;
    
    start = clock();
    sierpinskicarpet(img, 0, 0, W);
    end = clock();

    double duration = ((double)end - start) / CLOCKS_PER_SEC;

    svpng(fopen("fractal.png", "wb"), W, H, img, 0);

    printf("Fractal Generated. Time taken %f\n", duration);

    return 0;
}
