#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

typedef struct BMP {
    int height;
    int width;
    int size;
} BMP;

void NewGeneration(int **CurFile, int Height, int Width, int generation, int NumberIterations) {
    int NumberNeighbors;
    int i, j, x, y;
    int **HelpMas = (int **)malloc(sizeof(int *) * Height);
    for (i = 0; i < Height; i++) {
        HelpMas[i] = (int *)malloc(sizeof(int) * Width);
    }

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            HelpMas[i][j] = CurFile[i][j];
        }
    }

    for (y = 0; y < Height; y++) {
        for (x = 0; x < Width; x++) {
            NumberNeighbors = 0;
            for (i = -1; i < 2; i++) {
                for (j = -1; j < 2; j++) {
                    NumberNeighbors += CurFile [(Height + i + y) % Height] [(Width + j + x) % Width];
                }
            }

            if (HelpMas[y][x] == 1) {
                NumberNeighbors -= 1;
            }

            if (HelpMas[y][x] == 1) {
                if ((NumberNeighbors == 2) || (NumberNeighbors == 3)) {
                    HelpMas[y][x] = 1;
                }
                else {
                    HelpMas[y][x] = 0;
                }
            }
            else {
                if (NumberNeighbors == 3) {
                    HelpMas[y][x] = 1;
                }
                else {
                    HelpMas[y][x] = 0;
                }
            }
        }
    }

    int NumberOfCoincidences = 0;

    for (y = 0; y < Height; y++)
        for (x = 0; x < Width; x++) {
            if (CurFile[y][x] == HelpMas[y][x]) {
                NumberOfCoincidences++;
            }
            CurFile[y][x] = HelpMas[y][x];
        }

    for (i = 0; i < Height; i++) {
        free(HelpMas[i]);
    }
    free(HelpMas);

    if (NumberOfCoincidences == Height * Width) {
        printf("Program exit: same patern. Last generation %d", generation - 1);
        exit(0);
    }
    if (NumberIterations == generation) {
        printf("Program exit: Finish evolution %d", generation);
        return;
    }
}

void BitmapToArray(int height, int width, int **pict, unsigned char *byte, int div) {
    int d = 0;
    for (int i = height - 1; i >= 0; i--)
        for (int j = 0; j < width; j++) {
            if (byte[d] == 255) {
                pict[i][j] = 0;
            }
            else {
                pict[i][j] = 1;
            }
            d++;
            if (j == width - 1) {
                d += div;
            }
        }
}


void ArrayToBitmap(int height, int width, int **pict, unsigned char *byte, int div) {
    int d = 0;
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            if (pict[i][j] == 1) {
                byte[d] = 0;
            } else {
                byte[d] = 255;
            }
            d++;
            if (j == width - 1) {
                for (int t = 0; t < div; t++) {
                    byte[d] = 0;
                    d++;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    BMP Picture;
    unsigned char header[1078];
    int max_iter = INT_MAX;
    int dump_freq = 1;
    char* dir_name = NULL;

    FILE* file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp("--input", argv[i]) == 0) {
            file = fopen(argv[i + 1], "rb");
            i++;
        } else if (strcmp("--output", argv[i]) == 0) {
            dir_name = argv[i + 1];
            _mkdir(dir_name);
            i++;
        } else if (strcmp("--max_iter", argv[i]) == 0) {
            max_iter = strtol(argv[i + 1], 0, 10);
            i++;
        } else if (strcmp("--dump_freq", argv[i]) == 0) {
            dump_freq = strtol(argv[i + 1], 0, 10);
            i++;
        }
    }

    fread(header, sizeof(unsigned char), 1078, file);

    Picture.height = header[25] * 256 * 256 * 256 + header[24] * 256 * 256 + header[23] * 256 + header[22];
    Picture.width = header[21] * 256 * 256 * 256 + header[20] * 256 * 256 + header[19] * 256 + header[18];
    Picture.size = header[5] * 256 * 256 * 256 + header[4] * 256 * 256 + header[3] * 256 + header[2];

    unsigned char *byte = (unsigned char *)malloc((Picture.size - 1078) * sizeof(unsigned char));
    fread(byte, sizeof(unsigned char), Picture.size, file);

    int **pict = (int **)malloc(Picture.height * sizeof(int *));
    for (int i = 0; i < Picture.height; i++) {
        pict[i] = (int *)malloc(Picture.width * sizeof(int));
    }

    int div = (4 - Picture.width % 4) % 4;

    BitmapToArray(Picture.height, Picture.width, pict, byte, div);

    for (int g = 1; g <= max_iter; g++) {
        NewGeneration(pict, Picture.height, Picture.width, g, max_iter);

        if (g % dump_freq != 0) {
            continue;
        }

        char file_way[256];
        char file_name[16] = "gen_";
        char gen_number[16];

        strcpy(file_way, dir_name);
        strcat(file_way, "\\");
        strcat(file_name,_itoa(g, gen_number, 10));
        strcat(file_way, file_name);
        strcat(file_way, ".bmp");
        FILE* next_generation = fopen(file_way, "w");

        fwrite(header, 1, 1078, next_generation);

        ArrayToBitmap(Picture.height, Picture.width, pict, byte, div);

        fwrite(byte, sizeof(unsigned char), Picture.size, next_generation);
        fclose(next_generation);
    }

    free(byte);
    for (int i = 0; i < Picture.height; i++)
        free(pict[i]);
    free(pict);

    return 0;
}