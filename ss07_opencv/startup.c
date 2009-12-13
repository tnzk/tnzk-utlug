#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

int main(int argc, char **argv)
{
  char* i_filename = argv[1];
  char* o_filename = argv[2];
  IplImage *i = cvLoadImage(i_filename);
  IplImage *o = cvCloneImage(i);

  // Image Processing...

  cvSaveImage(o_filename, o);
  
  return 0;
}
