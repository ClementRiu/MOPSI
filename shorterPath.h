//
// Created by clementriu on 14/02/17.
//

#ifndef PROJECT_SHORTERPATH_H
#define PROJECT_SHORTERPATH_H

#include "library.h"
#include "imageProcessingFunc.h"

//! Initialise the disparity map.
Imagine::Image<int, 2> initDisparity(int largeur, int hauteur);

//! Initialise the cost map for a row
Imagine::Image<int, 2>
initCost(int largeur, int ligne, const Img &image1, const Img &image2);

//! shorterPath computation with dynamic programming
Imagine::Image<int, 1>
shorterPath(int largeur, const Imagine::Image<int, 2> &cost, int ligne);

//! depth computation
void disparityComputation(int largeur, const Img &image1,
                          const Img &image2, int ligne,
                          Imagine::Image<int, 2> &disparity, int &dispMax,
                          int &dispMin);

//! shorterPath computation with dynamic programming with edges
Imagine::Image<int, 1>
shorterPathEdgy(int largeur, const Imagine::Image<int, 2> &cost, int ligne,
                const Img &image1Edge, const Img &image2Edge);

//! depth computation with edges
void disparityComputationEdgy(int largeur, const Img &image1, const Img &image2,
                              int ligne, Imagine::Image<int, 2> &disparity,
                              int &dispMax, int &dispMin, const Img &image1Edge,
                              const Img &image2Edge);

#endif //PROJECT_SHORTERPATH_H
