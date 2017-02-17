//
// Created by clementriu on 14/02/17.
//

#ifndef PROJECT_IMAGEPROCESSINGFUNC_H
#define PROJECT_IMAGEPROCESSINGFUNC_H

#include "library.h"

typedef Imagine::Image<byte> Img;

//! Loads the images in image1 (left) and image2 (right).
int loadingImage(Img &image1, Img &image2, const std::string &name);

//! Find the edges of image using the Sobel method.
void edgeDetector(const Img &image, Img &imageOutput);

//! Gives a value between 0 and 254 to a given value.
byte transform(int valeur, int valMax, int valMin);

//! Gives back the depth map from a disparity map.
Imagine::Image<Imagine::Color, 2>
disparityToDepth(const Imagine::Image<int, 2> &disparity, int dispMax,
                 int dispMin, int largeur, int hauteur);

#endif //PROJECT_IMAGEPROCESSINGFUNC_H
