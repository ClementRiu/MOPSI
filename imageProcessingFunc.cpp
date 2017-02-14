//
// Created by clementriu on 14/02/17.
//

#include "imageProcessingFunc.h"

int loadingImage(Img &image1, Img &image2, std::string name) {
    //! Loads the images in image1 (left) and image2 (right).
    if (name == "cone") {
        if (!Imagine::load(image1, srcPath("cone/cone_L.png"))) {
            return 1;
        }
        if (!Imagine::load(image2, srcPath("cone/cone_R.png"))) {
            return 1;
        }
    } else {
        if (name == "tsuku") {
            if (!Imagine::load(image1, srcPath("tsukuba/tsuku_col1.ppm"))) {
                return 1;
            }
            if (!Imagine::load(image2, srcPath("tsukuba/tsuku_col5.ppm"))) {
                return 1;
            }
        }
    }
    // Checking the images are the same size.
    if (image1.width() == image2.width() &&
        image1.height() == image2.height()) {
        std::cout << "Chargement réussi." << std::endl;
        return 0;
    } else {
        return 1;
    }
}


void edgeDetector(Img image, Img &imageOutput) {
    //! Find the edges of image using the Sobel method.
    byte seuil = 64; // Si le gradient est en deça de ce seuil, on met le bord
    // en noir, sinon en blanc.

    int hauteur = image.height();
    int largeur = image.width();

    Imagine::Image<double, 2> imageDouble(largeur, hauteur);

    int valX, valY = 0;
    double valSqrt;
    double max = 0;

    int gX[3][3]; // Sobel horizontal gradient operator.
    int gY[3][3]; // Sobel vertical gradient operator.
    // x mask
    gX[0][0] = 1;
    gX[0][1] = 0;
    gX[0][2] = -1;
    gX[1][0] = 2;
    gX[1][1] = 0;
    gX[1][2] = -2;
    gX[2][0] = 1;
    gX[2][1] = 0;
    gX[2][2] = -1;
    // y mask
    gY[0][0] = 1;
    gY[0][1] = 2;
    gY[0][2] = 1;
    gY[1][0] = 0;
    gY[1][1] = 0;
    gY[1][2] = 0;
    gY[2][0] = -1;
    gY[2][1] = -2;
    gY[2][2] = -1;


    // Edge detection :
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {

            // The border is set to 0.
            if ((i == 0) || (i == hauteur - 1) || (j == 0) ||
                (j == largeur - 1)) {
                valX = 0;
                valY = 0;
            } else {
                // Computing the X and Y convolutions
                valX = valY = 0;
                for (int k = -1; k <= 1; ++k) {
                    for (int l = -1; l <= 1; ++l) {
                        valX = valX + image(j + l, i + k) * gX[1 + k][1 + l];
                        valY = valY + image(j + l, i + k) * gY[1 + k][1 + l];
                    }
                }
            }
            valSqrt = sqrt(valX * valX + 0 * valY * valY);
            // Setting the image value in a double image
            imageDouble(j, i) = valSqrt;

            if (valSqrt > max) {
                max = valSqrt;
            }
        }
    }

    // The obtain results is 0 or 255.
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; ++j) {
            if (imageDouble(j, i) > seuil * max / 255) {
                imageOutput(j, i) = 255;
            } else {
                imageOutput(j, i) = 0;
            }
        }
    }
}

byte transform(int valeur, int valMax, int valMin) {
    //! Gives a value between 0 and 254 to a given value.
    return byte(255 * (valeur - valMin + 1) / (valMax - valMin));
}

Imagine::Image<Imagine::Color, 2>
disparityToDepth(Imagine::Image<int, 2> disparity, int dispMax, int dispMin,
                 int largeur, int hauteur) {
    //! Gives back the depth map from a disparity map.
    Imagine::Image<Imagine::Color, 2> depth(largeur, hauteur);

    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            int localDisparity = disparity(j, i);
            if (localDisparity == -1) {
                depth(j, i) = Imagine::Color(155, 0, 0);
            } else {
                if (localDisparity == 0) {
                    depth(j, i) = Imagine::Color(0, 255, 255);
                } else {
                    byte col = transform(disparity(j, i), dispMax, dispMin);
                    depth(j, i) = Imagine::Color(0, col, col);
                }
            }
        }
    }
    return depth;
}