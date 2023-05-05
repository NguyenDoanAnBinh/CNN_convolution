#include <stdio.h>

#define IN_H 7
#define IN_W 7
#define IN_D 3

#define F_H 3
#define F_W 3
#define F_D 3

#define P 1
#define S 2

void conv(int X[IN_D][IN_H][IN_W], int W[F_D][F_H][F_W], int b, int Y[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1]) {
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
}

void conv_padding(int X1[IN_D][IN_H + 2*P][IN_W + 2*P], int W[F_D][F_H][F_W], int b, int Y[(IN_H + 2*P - F_H)/S + 1][(IN_W + 2*P - F_W)/S + 1]) {
    for (int i = 0; i < (IN_H + 2*P - F_H)/S + 1; i++) {
        for (int j = 0; j < (IN_H + 2*P - F_H)/S + 1; j++) {
            int sum = b;
            for (int k = 0; k < F_D; k++) {
                for (int r = 0; r < F_H; r++) {
                    for (int c = 0; c < F_W; c++) {
                        sum += X1[k][r + i * S][c + j * S] * W[k][r][c];
                    }
                }
            }
            Y[i][j] = sum;
        }
    }
}

void print_output(int Y[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1], const char* filter_name) {
    printf("\tOutput %s:\n", filter_name);
    for (int i = 0; i < (IN_H - F_H)/S + 1; i++) {
        for (int j = 0; j < (IN_H - F_H)/S + 1; j++) {
            printf("\t\t%d   ", Y[i][j]);
        }
        printf("\n");
    }
}

void print_output_padding(int Y[(IN_H + 2*P - F_H)/S + 1][(IN_W + 2*P - F_W)/S + 1], const char* filter_name) {
    printf("\tOutput %s:\n", filter_name);
    for (int i = 0; i < (IN_H + 2*P - F_H)/S + 1; i++) {
        for (int j = 0; j < (IN_H + 2*P - F_H)/S + 1; j++) {
            printf("\t\t%d   ", Y[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int W1[F_D][F_H][F_W] = { 
        {{0,1,1},
         {0,1,0},
         {0,1,0}},

        {{1,0,-1},
         {-1,0,0},
         {1,-1,1}},

        {{0,0,-1},
         {0,-1,-1},
         {0,-1,-1}}
    };
    int W2[F_D][F_H][F_W] = { 
        {{0,0,0},
         {1,0,1},
         {-1,0,-1}},

        {{0,0,1},
         {1,-1,-1},
         {1,-1,0}},

        {{1,1,0},
         {-1,0,0},
         {1,-1,1}}
    };
    int W3[F_D][F_H][F_W] = { 
        {{-1,-1,1},
         {0,1,-1},
         {0,1,1}},

        {{1,0,0},
         {1,-1,-1},
         {1,0,-1}},

        {{0,1,1},
         {0,1,0},
         {1,-1,1}}
    };
    
    int X[IN_D][IN_H][IN_W] = { 
		{{0,0,0,0,0,0,0},
         {0,1,1,0,0,2,0},
         {0,0,2,1,0,2,0},
         {0,2,0,1,2,2,0},
         {0,2,0,2,1,2,0},
         {0,0,0,1,1,2,0},
         {0,0,0,0,0,0,0}},

        {{0,0,0,0,0,0,0},
         {0,0,2,1,0,2,0},
         {0,0,2,1,2,2,0},
         {0,0,0,2,1,2,0},
         {0,1,0,0,0,2,0},
         {0,1,2,0,1,2,0},
         {0,0,0,0,0,0,0}},

        {{0,0,0,0,0,0,0},
         {0,1,2,1,2,0,0},
         {0,1,0,2,2,1,0},
         {0,1,0,0,0,1,0},
         {0,1,0,0,1,1,0},
         {0,0,2,1,0,1,0},
         {0,0,0,0,0,0,0}}
	};
	
    int X1[IN_D][IN_H + 2*P][IN_W + 2*P] = { 
        {{0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,1,1,0,0,2,0,0},
         {0,0,0,2,1,0,2,0,0},
         {0,0,2,0,1,2,2,0,0},
         {0,0,2,0,2,1,2,0,0},
         {0,0,0,0,1,1,2,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0}},

        {{0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,0,2,1,0,2,0,0},
         {0,0,0,0,2,1,2,0,0},
         {0,0,0,0,0,2,1,0,0},
         {0,0,1,0,0,0,2,0,0},
         {0,0,1,2,0,1,2,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0}},

        {{0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,1,2,1,2,0,0,0},
         {0,0,1,0,2,2,1,0,0},
         {0,0,1,0,0,0,1,0,0},
         {0,0,1,0,0,1,1,0,0},
         {0,0,0,2,1,0,1,0,0},
         {0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0}}
    };
    
    int Y1[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    int Y2[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    int Y3[(IN_H - F_H)/S + 1][(IN_W - F_W)/S + 1];
    
    int Y4[(IN_H + 2*P - F_H)/S + 1][(IN_W + 2*P - F_W)/S + 1];
    int Y5[(IN_H + 2*P - F_H)/S + 1][(IN_W + 2*P - F_W)/S + 1];
    int Y6[(IN_H + 2*P - F_H)/S + 1][(IN_W + 2*P - F_W)/S + 1];
    
    int b1 = 1;
    int b2 = 0;
    int b3 = 0;

	printf("No Padding\n");
    conv(X, W1, b1, Y1);
    conv(X, W2, b2, Y2);
    conv(X, W3, b3, Y3);

    print_output(Y1, "filter 1");
    printf("\n");
    print_output(Y2, "filter 2");
    printf("\n");
    print_output(Y3, "filter 3");
	
	printf("Have Padding\n");
    conv_padding(X1, W1, b1, Y4);
    conv_padding(X1, W2, b2, Y5);
    conv_padding(X1, W3, b3, Y6);

    print_output_padding(Y4, "filter 1");
    printf("\n");
    print_output_padding(Y5, "filter 2");
    printf("\n");
    print_output_padding(Y6, "filter 3");
    
    return 0;
}

