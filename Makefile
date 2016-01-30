PACKAGES=libtesseract3 libtesseract-dev libleptonica-dev

.PHONY: clean install_dependencies

all:
	g++ sudoku_ocr.cpp `pkg-config --libs --cflags tesseract lept` -o sudoku_ocr

install_dependencies:
	sudo apt-get update
	sudo apt-get install -y --no-install-recommends $(PACKAGES)

clean:
	rm sudoku_ocr

test:
	./runtests.sh
