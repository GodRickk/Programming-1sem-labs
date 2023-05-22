#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <math.h>

typedef struct parametrsFile {
   char fileName[255];
   int fileSize;
   int lengthFilename;
   unsigned char* bytes;
} parametrsFile;



struct parametrsFile readfile(char *name) {
    struct parametrsFile filepar;
    filepar.fileSize = 0;
    FILE* f = fopen(name, "rb");
    if (f == NULL) {
        printf("%s", "Файл отсутствует");
    }
    else {
        getc(f);
        while (!feof(f)) {
            getc(f);
            filepar.fileSize++;
        }
        fseek(f, 0, 0);
        int *array = malloc(filepar.fileSize * sizeof(int));
        filepar.bytes = malloc(filepar.fileSize * 2 * sizeof(char) + 1);
        for (int i = 0; i < filepar.fileSize; i++) {
            array[i] = (int)getc(f);
        }
        char array_16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        for (int i = 0; i < 2 * filepar.fileSize; i += 2) {
            filepar.bytes[i] = array_16[(array[i / 2] / 16) % 16];
            filepar.bytes[i + 1] = array_16[array[i / 2] % 16];
        }
        filepar.bytes[2 * filepar.fileSize] = '\0';
        filepar.fileSize *= 2;
        strcpy(filepar.fileName, name);
        filepar.lengthFilename = strlen(name);
    }
    fclose(f);
    return filepar;
}

void writeArc(int numberFiles, parametrsFile* a, char name[255]) {
    FILE *write_arc = fopen(name, "w");
    for (int j = 0; j < numberFiles; j++) {
        char helpmas[8];
        int length = a[j].lengthFilename;
        char array_16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        for (int i = 7; i >= 0; i--) {
            helpmas[i] = array_16[length % 16];
            length /= 16;
        }
        //printf("%d", sizeof(helpmas));


        fwrite(helpmas, 8, 1, write_arc);
        fwrite(a[j].fileName, a[j].lengthFilename * sizeof(char), 1, write_arc);

        int size = a[j].fileSize;
        for (int i = 7; i >= 0; i--) {
            helpmas[i] = array_16[size % 16];
            size /= 16;
        }
        fwrite(helpmas, 8, 1, write_arc);
        fwrite(a[j].bytes, a[j].fileSize * sizeof(char), 1, write_arc);
    }

    fclose(write_arc);

}

int find16(char c) {
    char array_16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 16; i++) {
        if (array_16[i] == c) {
            return i;
        }
    }
    return -1;
}

void showFiles (char name[255]) {
    FILE* ourArcive = fopen(name, "r");
    int numberbytes = 0;
    while (getc(ourArcive) != EOF) {
        fseek(ourArcive, -1, SEEK_CUR);
        char helpmas[8];
        fread(helpmas, 1, 8, ourArcive);
        int s = 0;
        for (int i = 7; i >= 0; i--) {
            s += find16(helpmas[i]) * (int) pow(16, 7 - i);
        }
        numberbytes += 8 + s;

        char filename[255];
        fread(filename, 1, s, ourArcive);
        filename[s] = '\0';

        fread(helpmas, 1, 8, ourArcive);
        s = 0;
        for (int i = 7; i >= 0; i--) {
            s += find16(helpmas[i]) * (int) pow(16, 7 - i);
        }
        numberbytes += 8 + s;
        //printf("%s %d\n", filename, s / 2);
        printf("%s \n", filename);
        char *buf = malloc (s * sizeof(char));
        fread(buf, 1, s, ourArcive);
        int k = 0;
    }
    fclose(ourArcive);
}


void out_of_arc(char name[255]) {
    FILE* our_arc = fopen(name, "r");
    while (getc(our_arc) != EOF) {
        fseek(our_arc, -1, SEEK_CUR);
        char helpmas[8];
        fread(helpmas, 1, 8, our_arc);
        int length = 0;
        for (int i = 7; i >= 0; i--) {
            length += find16(helpmas[i]) * (int)pow(16, 7 - i);
        }

        char *filename = malloc(sizeof(char) * (length + 1));
        fread(filename, 1, length, our_arc);
        filename[length] = '\0';

        fread(helpmas, 1, 8, our_arc);
        length = 0;
        for (int i = 7; i >= 0; i--) {
            length += find16(helpmas[i]) * (int)pow(16, 7 - i);
        }
        char *bytes = malloc(sizeof(char) * length);
        fread(bytes, 1, length, our_arc);
        FILE *file = fopen(filename, "w");
        for (int i = 0; i < length; i += 2) {
            int symbol = 0;
            symbol = find16(bytes[i + 1]) + find16(bytes[i]) * 16;
            char c[1];
            c[0] = (char)symbol;
            fwrite(c, 1, 1, file);

        }
        fclose(file);
    }
    fclose(our_arc);
}


int main(int argc, char *argv[]) {
    //printf("%d" , argc-4);
    if (!strcmp(argv[3], "--create")) {
        struct parametrsFile *array = malloc((argc - 4) * sizeof(parametrsFile));
        for (int i = 4; i < argc; i++) {
            array[i - 4] = readfile(argv[i]);
            //printf("%d %d %s %s \n", array[i - 4].fileSize, array[i - 4].lengthFilename, array[i - 4].fileName, array[i - 4].bytes);
        }
        writeArc(argc - 4 , array, argv[2]);
    }
    else if(!strcmp(argv[3], "--extract")) {
        out_of_arc(argv[2]);
    }
    else if (!strcmp(argv[3], "--list")) {
        showFiles(argv[2]);
    }
    else {
        printf("%s", "Eror. Unknown command");
    }

    return 0;
}