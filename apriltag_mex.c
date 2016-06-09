/*==========================================================
 * apriltag_mex.c - detect APRILTags in an image
 *
 * inputs:
 *  - image: grayscale image, HxW matrix
 * outputs:
 *  - id: 1xN matrix of the IDs of the tags found
 *  - center: 2xN matrix of the tag centers
 *  - p1: 2xN matrix of the upper left corners
 *  - p2: 2xN matrix of the lower left corners
 *  - p3: 2xN matrix of the lower right corners
 *  - p4: 2xN matrix of the upper right corners
 *  all except id are optional
 *
 * This is a MEX-file for MATLAB.
 * Copyright 2016 Alex Burka
 * BSD license
 *
 *========================================================*/

#include "mex.h"
#include "apriltag.h"
#include "tag36h11.h"
#include "image_u8.h"
#include "zarray.h"

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    uint8_t *image_matrix;               /* HxWx3 input matrix */
    image_u8_t *image;
    double *id = NULL, *center = NULL, *corner[4] = { NULL };
    size_t w, h;                   /* size of matrix */
    int i, j;
    apriltag_family_t *tf;
    apriltag_detector_t *td;
    zarray_t *detections;
    apriltag_detection_t *det;

    /* check for proper number of arguments */
    if (nrhs != 1) {
        mexErrMsgIdAndTxt("apriltag:input", "One input required (image matrix HxW)");
    }
    if (mxGetNumberOfDimensions(prhs[0]) != 2) {
        mexErrMsgIdAndTxt("apriltag:inputsize", "2D image required (did you forget rgb2gray)?");
    }
    if (!mxIsUint8(prhs[0])) {
        mexErrMsgIdAndTxt("apriltag:inputtype", "uint8 image required");
    }
    if (nlhs > 6) {
        mexErrMsgIdAndTxt("apriltag:output", "Too many outputs (available outputs are: id, center, p1-4)");
    }

    /* create a pointer to the real data in the input matrix  */
    image_matrix = mxGetData(prhs[0]);

    /* get dimensions of the input matrix */
    w = mxGetN(prhs[0]);
    h = mxGetM(prhs[0]);

    /* setup */
    tf = tag36h11_create();
    tf->black_border = 1;
    td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);
    td->quad_decimate = 1.0;
    td->quad_sigma = 0.0;
    td->nthreads = 4;
    td->debug = 0;
    td->refine_edges = 1;
    td->refine_decode = 0;
    td->refine_pose = 0;
    
    /* detect */
    image = image_u8_create_from_gray(w, h, image_matrix);
    detections = apriltag_detector_detect(td, image);

    /* create the output matrices */
    plhs[0] = mxCreateDoubleMatrix(1, zarray_size(detections), mxREAL); // ID
    id = mxGetPr(plhs[0]);
    if (nlhs > 1) {
        plhs[1] = mxCreateDoubleMatrix(2, zarray_size(detections), mxREAL); // center
        center = mxGetPr(plhs[1]);
        for (i = 0; i < 4; i++) {
            if (nlhs > 2+i) {
                plhs[2+i] = mxCreateDoubleMatrix(2, zarray_size(detections), mxREAL); // p
                corner[i] = mxGetPr(plhs[2+i]);
            }
        }
    }

    /* fill output matrices */
    for (i = 0; i < zarray_size(detections); i++) {
        zarray_get(detections, i, &det);

        id[i] = det->id;
        if (center) {
            center[i*2] = det->c[0];
            center[i*2+1] = det->c[1];
            for (j = 0; j < 4; j++) {
                if (corner[j]) {
                    corner[j][i*2] = det->p[j][0];
                    corner[j][i*2+1] = det->p[j][1];
                }
            }
        }
    }

    /* clean up */
    apriltag_detections_destroy(detections);
    image_u8_destroy(image);
    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);
}

