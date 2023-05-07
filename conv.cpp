#include <stdio.h>

#define IN_H 7
#define IN_W 7
#define IN_D 3

#define F_H 3
#define F_W 3
#define F_D 3

#define P 1
#define S 2

#define POOL_H 2
#define POOL_W 2

void conv(int X[IN_D][IN_H][IN_W], int W[F_D][F_H][F_W], int b, int Y[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1], const char* filter_name) {
    for (int i = 0; i < (IN_H - F_H)/S + 1; i++) {
        for (int j = 0; j < (IN_H - F_H)/S + 1; j++) {
            int sum = b;
            for (int k = 0; k < F_D; k++) {
                for (int r = 0; r < F_H; r++) {
                    for (int c = 0; c < F_W; c++) {
                        sum += X[k][r + i * S][c + j * S] * W[k][r][c];
                    }
                }
            }
            Y[i][j] = sum;
        }
    }
    
    printf("\tOutput %s:\n", filter_name);
    for (int i = 0; i < (IN_H - F_H)/S + 1; i++) {
        for (int j = 0; j < (IN_H - F_H)/S + 1; j++) {
            printf("\t%2d", Y[i][j]);
        }
        printf("\n\n");
    }
}

void print_padded_output(int Y[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1], const char* filter_name, int pad_val) {
    int out_h = (IN_H - F_H)/S + 1;
    int out_w = (IN_W - F_W)/S + 1;
    
    int pad_h = out_h + 2*P;
    int pad_w = out_w + 2*P;
    
    int padded[pad_h][pad_w];
    
    for (int i = 0; i < pad_h; i++) {
        for (int j = 0; j < pad_w; j++) {
            padded[i][j] = pad_val;
        }
    }

    for (int i = 0; i < out_h; i++) {
        for (int j = 0; j < out_w; j++) {
            padded[i+P][j+P] = Y[i][j];
        }
    }
    
    printf("\tOutput %s:\n", filter_name);
    for (int i = 0; i < pad_h; i++) {
        for (int j = 0; j < pad_w; j++) {
            printf("\t%2d", padded[i][j]);
        }
        printf("\n\n");
    }
}

void max_pooling(int Y[(IN_H - F_H) / S + 1][(IN_W - F_W)/S + 1], int pool_Y[(IN_H - F_H - POOL_H)/S + 1][(IN_W - F_W - POOL_W)/S + 1], const char *filter_name) {
    for (int i = 0; i < (IN_H - F_H - POOL_H)/S + 1; i++) {
        for (int j = 0; j < (IN_W - F_W - POOL_W)/S + 1; j++) {
            int max_value = Y[i][j];
            for (int r = 0; r < POOL_H; r++) {
                for (int c = 0; c < POOL_W; c++) {
                    if (Y[i + r][j + c] > max_value) {
                        max_value = Y[i + r][j + c];
                    }
                }
            }
            pool_Y[i][j] = max_value;
        }
    }
    
    printf("\tOutput %s:\n", filter_name);
    for (int i = 0; i < (IN_H - F_H - POOL_H)/S + 1; i++) {
        for (int j = 0; j < (IN_W - F_W - POOL_W)/S + 1; j++) {
            printf("\t%d   ", pool_Y[i][j]);
        }
        printf("\n\n");
    }
}

void read_input(int in[IN_D][IN_H][IN_W], const char* file_name) {
    FILE* file = fopen(file_name, "r");
    for (int d = 0; d < IN_D; d++) {
        for (int i = 0; i < IN_H; i++) {
            for (int j = 0; j < IN_W; j++) {
                fscanf(file, "%d", &in[d][i][j]);
            }
        }
    }
    fclose(file);
}

void read_filter(int f[F_D][F_H][F_W], const char* file_name) {
    FILE* file = fopen(file_name, "r");
    for (int d = 0; d < F_D; d++) {
        for (int i = 0; i < F_H; i++) {
            for (int j = 0; j < F_W; j++) {
                fscanf(file, "%d", &f[d][i][j]);
            }
        }
    }
    fclose(file);
}

int main() {
    int W1[F_D][F_H][F_W];
    read_filter(W1, "w1.txt");
    int W2[F_D][F_H][F_W];
    read_filter(W2, "w2.txt");
    int W3[F_D][F_H][F_W];
    read_filter(W3, "w3.txt");
    
    int X[IN_D][IN_H][IN_W];
    read_input(X, "input.txt");
    
    int Y1[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    int Y2[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    int Y3[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    
    int b1 = 1;
    int b2 = 0;
    int b3 = 0;

	printf("No Padding:\n");
    conv(X, W1, b1, Y1, "filter 1");
    conv(X, W2, b2, Y2, "filter 2");
    conv(X, W3, b3, Y3, "filter 3");
	
	int pad_val = 0;
	printf("With Padding:\n");
	print_padded_output(Y1, "filter 1", pad_val);
	print_padded_output(Y2, "filter 2", pad_val);
	print_padded_output(Y3, "filter 3", pad_val);
	
	int pool_Y1[(IN_H - F_H - POOL_H)/S + 1][(IN_W - F_W - POOL_W)/S + 1];
	int pool_Y2[(IN_H - F_H - POOL_H)/S + 1][(IN_W - F_W - POOL_W)/S + 1];
	int pool_Y3[(IN_H - F_H - POOL_H)/S + 1][(IN_W - F_W - POOL_W)/S + 1];

	printf("Max Pooling:\n");
    max_pooling(Y1, pool_Y1, "filter 1");
    max_pooling(Y2, pool_Y2, "filter 2");
    max_pooling(Y3, pool_Y3, "filter 3");
    
    return 0;
}
