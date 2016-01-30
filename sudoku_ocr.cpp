#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int sudoku_glyph_w = 18;
int sudoku_glyph_h = 28;
int sudoku_w = 9;
int sudoku_h = 9;
int sudoku_box_w = 364/9;
int sudoku_box_h = 364/9;
char *sudoku_file = NULL;
int tobsan_format = 0;


void usage(char *name) {
    printf("Usage: %s [OPTION(S)]\n", name);
    printf("Required switches:\n");
    printf("  -w Number of sudoku cells in width\n");
    printf("  -h Number of sudoku cells in height\n");
    printf("  -a Glyph width in pixels\n");
    printf("  -b Glyph height in pixels\n");
    printf("  -c Sudoku box width in pixels\n");
    printf("  -d Sudoku box height in pixels\n");
    printf("  -f Sudoku image file\n");
    printf("\nOptional switches:\n");
    printf("  -t Tobsan output format\n");
}

void display_tobsan(char *board, int len) {
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (board[i] == ' ') {
            printf(".");
        } else {
            printf("%c", board[i]);
        }

        if (j++ == 8) {
            j = 0;
            printf(" ");
        }
    }
    printf("\n");
}

void display_board(char *board, int width, int height) {
    int i = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c ", board[i++]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int c = 0;
    while ((c = getopt (argc, argv, "w:h:a:b:c:d:f:t")) != -1) {
        switch (c) {
            case 'w':
                sudoku_w = atoi(optarg);
                break;
            case 'h':
                sudoku_h = atoi(optarg);
                break;
            case 'a':
                sudoku_glyph_w = atoi(optarg);
                break;
            case 'b':
                sudoku_glyph_h = atoi(optarg);
                break;
            case 'c':
                sudoku_box_w = atoi(optarg) / 9;
                break;
            case 'd':
                sudoku_box_h = atoi(optarg) / 9;
                break;
            case 'f':
                sudoku_file = strndup(optarg, 1024);
                break;
            case 't':
                tobsan_format = 1;
                break;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if (!sudoku_file) {
        usage(argv[0]);
        return 1;
    }

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead(sudoku_file);
    api->SetImage(image);
    int x = 0, y = 0;
    char *board = (char *)calloc(sudoku_w * sudoku_h, sizeof(*board));
    int i = 0;

    for (y = 1; y < sudoku_h + 1; y++) {
        for (x = 1; x < sudoku_w + 1; x++) {
            char *outText;
            int x0 = (sudoku_box_w * x) - (sudoku_box_w / 2) - (sudoku_glyph_w / 2);
            int y0 = (sudoku_box_h * y) - (sudoku_box_h / 2) - (sudoku_glyph_h / 2);
            int x1 = sudoku_glyph_w;
            int y1 = sudoku_glyph_h;
            api->SetRectangle(x0, y0, x1, y1);
            // Get OCR result
            outText = api->GetUTF8Text();

            if (strncmp(outText, "", 1) != 0) {
                board[i++] = outText[0];
            } else {
                board[i++] = ' ';
            }
            delete [] outText;
        }
    }

    if (tobsan_format) {
        display_tobsan(board, sudoku_h * sudoku_w);
    } else {
        display_board(board, sudoku_w, sudoku_h);
    }

    // Destroy used object and release memory
    api->End();
    pixDestroy(&image);

    return 0;
}
