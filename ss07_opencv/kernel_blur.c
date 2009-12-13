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
  CvMat* kernel = cvCreateMat( 3, 3, CV_32F);

  // Getting the image data from input data
  height     = i_img->height;
  width      = i_img->width;
  step       = i_img->widthStep;
  n_channels = i_img->nChannels;
  i_data     = (unsigned char*)i_img->imageData;
  o_data     = (unsigned char*)o_img->imageData;

  // Setting up the 3x3 matrix kernel
  cvmSet( kernel, 0, 0, 0.11111);
  cvmSet( kernel, 0, 1, 0.11111);
  cvmSet( kernel, 0, 2, 0.11111);
  cvmSet( kernel, 1, 0, 0.11111);
  cvmSet( kernel, 1, 1, 0.11111);
  cvmSet( kernel, 1, 2, 0.11111);
  cvmSet( kernel, 2, 0, 0.11111);
  cvmSet( kernel, 2, 1, 0.11111);
  cvmSet( kernel, 2, 2, 0.11111);

  for( i = 0; i < height; i++){
    for( j = 0; j < width; j++){
      unsigned char* o_pix = o_data + (*y) * step + (*x) * n_channels;
      unsigned char* i_pix = i_data + (*y) * step + (*x) * n_channels;
      int ii;
      int jj;
      double rsum = 0.0;
      double gsum = 0.0;
      double bsum = 0.0;
      for( jj = 0; jj < 3; jj++){
	for( ii = 0; ii < 3; ii++){
	  double k = cvmGet( kernel, ii, jj);
	  double r = *(i_data + (*y + ii - 1) * step + (*x + jj + 1) * n_channels + 0);
	  double g = *(i_data + (*y + ii - 1) * step + (*x + jj + 1) * n_channels + 1);
	  double b = *(i_data + (*y + ii - 1) * step + (*x + jj + 1) * n_channels + 2);
	  rsum += k * r;
	  gsum += k * g;
	  bsum += k * b;
	}
      }

      *(o_pix + 0) = (int)rsum;
      *(o_pix + 1) = (int)gsum;
      *(o_pix + 2) = (int)bsum;
    }
  }


  cvSaveImage(o_filename, o_img);
  
  puts("done.");

  return 0;
}
