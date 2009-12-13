#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  int i, j;
  int *x = &j;
  int *y = &i;
  int width;
  int height;
  int step;
  int n_channels;
  char* i_filename = argv[1];
  char* o_filename = argv[2];
  unsigned char* i_data;
  unsigned char* o_data;
  IplImage *i_img = cvLoadImage(i_filename);
  IplImage *o_img = cvCloneImage(i_img);

  // Getting the image data from input data
  height     = i_img->height;
  width      = i_img->width;
  step       = i_img->widthStep;
  n_channels = i_img->nChannels;
  i_data     = (unsigned char*)i_img->imageData;
  o_data     = (unsigned char*)o_img->imageData;

  for( i = 0; i < height; i++){
    for( j = 0; j < width; j++){
      unsigned char* o_pix = o_data + (*y) * step + (*x) * n_channels;
      unsigned char* i_pix = i_data + (*y) * step + (*x) * n_channels;
      int avg = (*(i_pix + 0) + *(i_pix + 1) + *(i_pix + 2)) / 3;
      if(*(i_pix + 2) > 150){

      }else{
	*(o_pix + 0) = 0;
	*(o_pix + 1) = 0;
	*(o_pix + 2) = 0;
      }
    }
  }


  cvSaveImage(o_filename, o_img);
  
  puts("done.");

  return 0;
}
