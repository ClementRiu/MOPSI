//
// Created by clementriu on 14/02/17.
//

#ifndef PROJECT_SHORTERPATH_H
#define PROJECT_SHORTERPATH_H

#include "imageProcessingFunc.h"

//! Initialise the disparity map.
Imagine::Image<int, 2> initDisparity(int largeur, int hauteur);

//! Initialise the cost map for a row
Imagine::Image<int, 2> initCost(int largeur, int ligne);

//! shorterPath computation with dynamic programming
Imagine::Image<int, 1>
shorterPath(int largeur, int hauteur, Img image1, Img image2, int ligne);

//! depth computation
void disparityComputation(int largeur, int hauteur, Img image1, Img image2,
                          int ligne, Imagine::Image<int, 2> disparity,
                          int dispMax, int dispMin);

#endif //PROJECT_SHORTERPATH_H
