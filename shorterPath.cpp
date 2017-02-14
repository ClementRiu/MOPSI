//
// Created by clementriu on 14/02/17.
//

#include "shorterPath.h"


Imagine::Image<int, 2> initDisparity(int largeur, int hauteur) {
    //! Initialise the disparity map.
    Imagine::Image<int, 2> disparity(largeur, hauteur);
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            disparity(j, i) = -1;
        }
    }
    return disparity;
}


Imagine::Image<int, 2>
initCost(int largeur, int ligne, Img image1, Img image2) {
    //! Initialise the cost map for a row
    Imagine::Image<int, 2> cost(largeur, largeur);
    for (int j = 0; j < largeur; ++j) {
        for (int k = 0; k < largeur; ++k) {
            cost(k, j) = abs(image1(k, ligne) - image2(j, ligne));
        }
    }
    return cost;
};


Imagine::Image<int, 1>
shorterPath(int largeur, int hauteur, Img image1, Img image2, int ligne) {
    //! shorterPath computation with dynamic programming
    // assiociation gives the pixels associated to the corresponding pixel.
    Imagine::Image<int, 1> assiociation(largeur);
    for (int j = 0; j < largeur; ++j) {
        assiociation(j) = -1; // -1 means there are no association.
    }

    Imagine::Image<float, 2> distance(largeur, largeur); // distance[i][j] sera
    // la distance minimale permettant associer les pixels situés dans
    // (ligne,:i) dans l'image gauche avec les pixels situés dans (ligne,:j)
    // dans l'image droite.

    Imagine::Image<int, 2> cost = initCost(largeur, ligne, image1, image2);

    //! Initial distance computation :
    for (int i = 0; i < largeur; ++i) {
        distance(0, i) = cost(0, i);
        distance(i, 0) = cost(i, 0);
    }

    //! Computation between prefixes
    for (int i = 1; i < largeur; ++i) {
        for (int j = 1; j < largeur; j++) {
            distance(j, i) = std::min(
                    std::min(distance(j, i - 1), distance(j - 1, i)),
                    distance(j - 1, i - 1) + cost(j, i));
        }
    }

    int realDistance = distance(largeur - 1, largeur - 1);

    std::pair<int, int> etape = std::make_pair(largeur - 1, largeur - 1);
    assiociation(etape.first) = etape.second;
    //on suppose que les deux derniers pixels sont toujours associés ???? A REVOIR !!!
    //etape est un couple d'indices caractérisant le nouveau sous-problème auquel on se ramène, qui appartient au chemin optimal
    //si etape=(i,j), le prochain sous-problème du chemin optimal consistera à évaluer la distance entre s[:i-1] et t[:j-1]

    while ((etape.first > 0) && (etape.second > 0)) {
        float D_antecedent = distance(etape.second, etape.first) -
                             cost(etape.second, etape.first);
        if (D_antecedent == distance(etape.second- 1, etape.first- 1)) {
            etape.first--;
            etape.second--;
        } else {
            if (D_antecedent == distance(etape.second- 1, etape.first)) {
                etape.first--;
            } else {
                if (D_antecedent == distance(etape.second, etape.first- 1)) {
                    etape.second--;
                }
            }
        }
        assiociation(etape.first) = etape.second;
    }
    return assiociation;
}

void disparityComputation(int largeur, int hauteur, Img image1, Img image2,
                          int ligne, Imagine::Image<int, 2> disparity,
                          int dispMax, int dispMin) {
    //! depth computation
    Imagine::Image<int, 1> association = shorterPath(largeur, hauteur, image1,
                                                     image2, ligne);

    for (int j = 0; j < largeur; ++j) {
        if (association(j) != -1) {
            int localDisparity = abs(j - association(j));
            disparity(j, ligne) = abs(j - association(j));
            if (localDisparity > dispMax) {
                dispMax = localDisparity;
            }
            if (localDisparity < dispMin) {
                dispMin = localDisparity;
            }
        }

    }
}