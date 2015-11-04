//#define DEB
#ifdef DEB
	#define DEBUG(x) {cout << x << endl;}
	#define PRINT_MAT(mat,NH,NW) {for (int i = 0; i < NH; ++i) {for (int j = 0; j < NW; ++j) {cout << (int)mat[i*NW + j]; } cout << endl; }}
#else
	#define DEBUG(x)
	#define PRINT_MAT(mat,NH,NW)
#endif
#define CALL_LabelComponent(x,y,returnLabel) { STACK[SP] = x; STACK[SP+1] = y; STACK[SP+2] = returnLabel; SP += 3; goto START; }
#define RETURN { SP -= 3;                \
                 switch (STACK[SP+2])    \
                 {                       \
                 case 1 : goto RETURN1;  \
                 case 2 : goto RETURN2;  \
                 case 3 : goto RETURN3;  \
                 case 4 : goto RETURN4;  \
                 case 5 : goto RETURN5;  \
                 case 6 : goto RETURN6;  \
                 case 7 : goto RETURN7;  \
                 case 8 : goto RETURN8;  \
                 default: return;        \
                 }                       \
               }
#define RETURN_4 { SP -= 3;                \
                 switch (STACK[SP+2])    \
                 {                       \
                 case 1 : goto RETURN1;  \
                 case 2 : goto RETURN2;  \
                 case 3 : goto RETURN3;  \
                 case 4 : goto RETURN4;  \
                 default: return;        \
                 }                       \
               }
#define X (STACK[SP-3])
#define Y (STACK[SP-2])
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
using namespace std;




void LabelComponent(unsigned short* STACK, unsigned short width, unsigned short height, unsigned char* input, int* output, int labelNo, unsigned short x, unsigned short y)
{
  STACK[0] = x;
  STACK[1] = y;
  STACK[2] = 0;  /* return - component is labelled */
  int SP   = 3;
  int index;

START: /* Recursive routine starts here */

  index = X + width*Y;
  if (input [index] == 0) RETURN;   /* This pixel is not part of a component */
  if (output[index] != 0) RETURN;   /* This pixel has already been labelled  */
  output[index] = labelNo;

  if (X > 0) CALL_LabelComponent(X-1, Y, 1);   /* left  pixel */
RETURN1:
	if (X > 0 && Y > 0) CALL_LabelComponent(X-1, Y-1, 5);	// upper left
RETURN5:
	if (X > 0 && Y < height-1) CALL_LabelComponent(X-1, Y+1, 6);	// lower left
RETURN6:

  if (X < width-1) CALL_LabelComponent(X+1, Y, 2);   /* right pixel */
RETURN2:
	if (X < width-1 && Y > 0) CALL_LabelComponent(X+1, Y-1,7);	// upper right
RETURN7:
	if (X < width-1 && Y < height-1) CALL_LabelComponent(X+1, Y+1, 8);
RETURN8:

  if (Y > 0) CALL_LabelComponent(X, Y-1, 3);   /* upper pixel */
RETURN3:

  if (Y < height-1) CALL_LabelComponent(X, Y+1, 4);   /* lower pixel */
RETURN4:

  RETURN;
}

void LabelComponent_4(unsigned short* STACK, unsigned short width, unsigned short height, unsigned char* input, int* output, int labelNo, unsigned short x, unsigned short y)
{
  STACK[0] = x;
  STACK[1] = y;
  STACK[2] = 0;  /* return - component is labelled */
  int SP   = 3;
  int index;

START: /* Recursive routine starts here */

  index = X + width*Y;
  if (input [index] == 0) RETURN_4;   /* This pixel is not part of a component */
  if (output[index] != 0) RETURN_4;   /* This pixel has already been labelled  */
  output[index] = labelNo;

  if (X > 0) CALL_LabelComponent(X-1, Y, 1);   /* left  pixel */
RETURN1:

  if (X < width-1) CALL_LabelComponent(X+1, Y, 2);   /* right pixel */
RETURN2:

  if (Y > 0) CALL_LabelComponent(X, Y-1, 3);   /* upper pixel */
RETURN3:

  if (Y < height-1) CALL_LabelComponent(X, Y+1, 4);   /* lower pixel */
RETURN4:

  RETURN_4;
}

void LabelImage(unsigned short width, unsigned short height, unsigned char* input, int* output)
{
  unsigned short* STACK = (unsigned short*) malloc(3*sizeof(unsigned short)*(width*height + 1));
  
  int labelNo = 0;
  int index   = -1;
  for (unsigned short y = 0; y < height; y++)
  {
    for (unsigned short x = 0; x < width; x++)
    {
      index++;
      if (input [index] == 0) continue;   /* This pixel is not part of a component */
      if (output[index] != 0) continue;   /* This pixel has already been labelled  */
      /* New component found */
      labelNo++;
      LabelComponent(STACK, width, height, input, output, labelNo, x, y);
    }
  }

  free(STACK);
}

void LabelImage_4(unsigned short width, unsigned short height, unsigned char* input, int* output)
{
  unsigned short* STACK = (unsigned short*) malloc(3*sizeof(unsigned short)*(width*height + 1));
  
  int labelNo = 0;
  int index   = -1;
  for (unsigned short y = 0; y < height; y++)
  {
    for (unsigned short x = 0; x < width; x++)
    {
      index++;
      if (input [index] == 0) continue;   /* This pixel is not part of a component */
      if (output[index] != 0) continue;   /* This pixel has already been labelled  */
      /* New component found */
      labelNo++;
      LabelComponent_4(STACK, width, height, input, output, labelNo, x, y);
    }
  }

  free(STACK);
}


int main() {
	int T;
	cin >> T;
	while (T--) {
		int H,W;
		cin >> H >> W;
		unsigned char* input  = (unsigned char*) malloc(W*H*sizeof(unsigned char));
		unsigned char* paddedinput  = (unsigned char*) malloc((W+2)*(H+2)*sizeof(unsigned char));
		int*           foreground = (int*)           malloc(W*H*sizeof(int));
		int*           background = (int*)           malloc((W+2)*(H+2)*sizeof(int));
		memset(foreground, 0, W*H*sizeof(int));
		memset(background, 0, (W+2)*(H+2)*sizeof(int));
		// initialize input
		char c;
		for (int i = 0; i < H; ++i) {
			for (int j = 0; j < W; ++j) {
				cin >> c;
				if (c == 'O') input[i*W + j] = 0;
				else if (c == 'X') input[i*W + j] = 1;
			}
		}
		LabelImage(W,H,input,foreground);
		// borders
		for (int i = 0; i < H+2; ++i) {
			paddedinput[i*(W+2)] = 1;
			paddedinput[i*(W+2) + W+1] = 1;
		}
		for (int j = 0; j < W+2; ++j) {
			paddedinput[j] = 1;
			paddedinput[(H+1)*(W+2) + j] = 1;
		}
		for (int i = 1; i < H+1; ++i) {
			for (int j = 1; j < W+1; ++j) {
				if (input[(i-1)*W + (j-1)] == 0) paddedinput[i*(W+2) + j] = 1;
				else paddedinput[i*(W+2) + j] = 0;
			}
		}
		// PRINT_MAT(paddedinput,(H+2),(W+2));
		LabelImage_4(W+2,H+2,paddedinput,background);
		// PRINT_MAT(input,H,W);
		// DEBUG(' ');
		PRINT_MAT(foreground,H,W);
		// DEBUG(' ');
		PRINT_MAT(background,(H+2),(W+2));

		int num_foreground = 0, num_background = 0;
		for (int i = 0; i < H; ++i) {
			for (int j = 0; j < W; ++j) {
				if (foreground[i*W + j] > num_foreground) num_foreground = foreground[i*W + j];
			}
		}
		for (int i = 0; i < H+2; ++i) {
			for (int j = 0; j < W+2; ++j) {
				if (background[i*(W+2) + j] > num_background) num_background = background[i*(W+2) + j];
			}
		}
		DEBUG("foreground " << num_foreground << " holes " << num_background - 1);
		cout << num_foreground - num_background + 1 << endl;
	}
}