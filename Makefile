BUILD_DIR = build
INC_DIR = inc
SRC_DIR = src

all:
	gcc \
		-Wall -Wextra \
		-O3 -march=native -mtune=native \
		-fomit-frame-pointer \
		-funroll-loops -ftree-vectorize \
		$(SRC_DIR)/* \
		-o $(BUILD_DIR)/main \

clean:
	rm -f $(BUILD_DIR)/*