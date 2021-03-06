#include "imageProcessing.h"
#include "globalVars.h"
#include "routines.h"

/*
 * blockMatchingFunction 
 */

double computeMatch(unsigned char *im,
		    int im_step,
		    unsigned char *bl,
		    int bl_step,
		    int bl_cols,
		    int bl_rows,
		    int oi, 
		    int oj, 
		    int stride){
  
  if (!im || !bl) return 0.0;

  double nb = (bl_cols*bl_rows);
  double x = 0;
  for(int i = 0;i < bl_rows-stride+1;i+= stride){
    for(int j = 0;j < bl_cols-stride+1;j+= stride){
      unsigned char v1 = im[INDXs(im_step,oi+i,oj+j)];
      unsigned char v2 = bl[INDXs(bl_step,i,j)];
      x += (v2-v1)*(v2-v1);
      //im[INDXs(im_step,oi+i,oj+j)] = ABS(v2-v1);
    }
  }
  x = x / nb;
  //  printf("%f\n",x);
  return x;
}

double blockMatching(cv::Mat *image,
		     cv::Mat *block,
		     int stride,
		     unsigned char *res,
		     int samplenum){
  
  if (!image || !block) return DBL_MAX;
  unsigned char *bl = (unsigned char*)(block->data);
  int bl_step = block->step;
  int bl_cols = block->cols;
  int bl_rows = block->rows;

  unsigned char *im = (unsigned char*)(image->data);
  int im_step = image->step;
  int im_cols = image->cols;
  int im_rows = image->rows;

  int coord_i_min = 0;
  int coord_j_min = 0;

  double minVal =  DBL_MAX;
  int istart = 0;
  int iend =  im_rows - bl_rows;
  int jstart = 0;
  int jend =  im_cols - bl_cols;

  for(int i = istart;i < iend -stride+1;i+=stride){
    for(int j = jstart;j < jend-stride+1;j+=stride){
      double x = computeMatch(im,im_step,
			      bl,bl_step,bl_cols,bl_rows,
			      i,j,stride);
      if(x < minVal){
	minVal = x;
	coord_i_min = i;
	coord_j_min = j;
      }
    }
  }

  if (Verbose)   fprintf(stderr,"sample cols: %d\n",bl_cols);
  if (Verbose)   fprintf(stderr,"sample rows: %d\n",bl_rows);
  if (Verbose)   fprintf(stderr,"sample step: %d\n",bl_step);
  if (Verbose)   fprintf(stderr,"image cols: %d\n",im_cols);
  if (Verbose)   fprintf(stderr,"image rows: %d\n",im_rows);
  if (Verbose)   fprintf(stderr,"image step: %d\n",im_step);

  memcpy(&(res[0]),&coord_i_min,sizeof(int));
  memcpy(&(res[4]),&coord_j_min,sizeof(int));
  memcpy(&(res[8]),&minVal,sizeof(double));
  memcpy(&(res[16]),&samplenum,sizeof(int));

  if (Verbose) fprintf(stderr,"%d sample x=%d, y=%d --> %f \n",samplenum, coord_j_min,coord_i_min,minVal); 
  return minVal;
}



double blockMatchingWithScalingAndRotation(cv::Mat *image,
		     cv::Mat *blocki,
		     int stride,
		     unsigned char *res,
		     int samplenum){
  
  if (!image || !blocki) return DBL_MAX;

  //this code has memory leaks...

  unsigned char *im = (unsigned char*)(image->data);
  int im_step = image->step;
  int im_cols = image->cols;
  int im_rows = image->rows;
  
  int coord_i_min = 0;
  int coord_j_min = 0;

  double minVal =  DBL_MAX;
  float bestScale = 0;
  int bestRotation = 0;
    
  for (int r = -10; r < 10; r = r+2){
    printf("Trying rotation %d\n",r);
    cv::Mat *rot = rotateImage(blocki,r);
    for (float s = 1.0; s > 0.3; s = s-0.1){
      printf("Trying scaling %f\n",s);
      cv::Mat *block = scaleImage(rot,s);
      showOneImage(*block);      
      
      unsigned char *bl = (unsigned char*)(block->data);
      int bl_step = block->step;
      int bl_cols = block->cols;
      int bl_rows = block->rows;      
      
      int istart = 0;
      int iend =  im_rows - bl_rows;
      int jstart = 0;
      int jend =  im_cols - bl_cols;
      
      for(int i = istart;i < iend -stride+1;i+=stride){
	for(int j = jstart;j < jend-stride+1;j+=stride){
	  double x = computeMatch(im,im_step,
				  bl,bl_step,bl_cols,bl_rows,
				  i,j,stride);
	  if(x < minVal){
	    minVal = x;
	    coord_i_min = i;
	    coord_j_min = j;
	    bestScale = s;
	    bestRotation = r;

	  }
	}
      }
      
      if (Verbose)   fprintf(stderr,"sample cols: %d\n",bl_cols);
      if (Verbose)   fprintf(stderr,"sample rows: %d\n",bl_rows);
      if (Verbose)   fprintf(stderr,"sample step: %d\n",bl_step);
      if (Verbose)   fprintf(stderr,"image cols: %d\n",im_cols);
      if (Verbose)   fprintf(stderr,"image rows: %d\n",im_rows);
      if (Verbose)   fprintf(stderr,"image step: %d\n",im_step);
      if (Verbose)   fprintf(stderr,"Current score: %f\n",minVal);
      delete block; 
    }
    delete rot;
  }
  memcpy(&(res[0]),&coord_i_min,sizeof(int));
  memcpy(&(res[4]),&coord_j_min,sizeof(int));
  memcpy(&(res[8]),&minVal,sizeof(double));
  memcpy(&(res[16]),&samplenum,sizeof(int));
    
  if (Verbose) fprintf(stderr,"%d sample x=%d, y=%d --> %f (scale %f, rot %d) \n",
		       samplenum, coord_j_min,coord_i_min,minVal,bestScale,bestRotation); 
  return minVal;
}

