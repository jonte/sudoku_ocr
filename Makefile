.PHONY: clean

all:
	g++ sudoku_ocr.cpp `pkg-config --libs --cflags tesseract lept` -o sudoku_ocr

clean:
	rm sudoku_ocr
