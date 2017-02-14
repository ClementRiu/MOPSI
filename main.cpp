//
// Created by clementriu on 14/02/17.
//

#include "imageProcessingFunc.h"
#include "shorterPath.h"

int main() {
    //!Chargement et affichage des images.
    Img image_L;
    Img image_R;
    // Chargement des images.
    // "cone" ou "tsuku"
    if (loadingImage(image_L, image_R, "cone") == 1) {
        std::cerr << "Mauvais chargement de fichier" << std::endl;
        return 0;
    }

    // Affichage d'information sur les images.
    int largeur = image_L.width(); // Identique pour les deux images
    int hauteur = image_L.height();
    std::cout << "Dimension de l'image : " << largeur << " * " << hauteur
              << std::endl;
    int taille = largeur * hauteur;
    std::cout << taille << std::endl;

    // Affichage des images.
    Imagine::openWindow(3 * largeur, 2 * hauteur, "Images");
    display(grey(image_L));
    display(grey(image_R), Imagine::IntPoint2(0, hauteur));

    //! Affichage des bords des images
    Imagine::Image<byte, 2> image_L_edges(largeur, hauteur);
    edgeDetector(image_L, image_L_edges);
    display(grey(image_L_edges), Imagine::IntPoint2(largeur, 0));
    Imagine::Image<byte, 2> image_R_edges(largeur, hauteur);
    edgeDetector(image_R, image_R_edges);
    display(grey(image_R_edges), Imagine::IntPoint2(largeur, hauteur));

    //! Calcul de la carte de disparité
    Imagine::Image<int, 2> disparity(largeur, hauteur);
    disparity = initDisparity(largeur, hauteur);
    int dispMax = -1;
    int dispMin = INFINITY;

    // Time calculation to compare different algorithm
    struct timespec start, finish;
    double elapsed;
    std::cout << "Début du calcul..." << std::endl;
    clock_t time1 = clock_gettime(CLOCK_MONOTONIC, &start);

#pragma omp parallel for
    for (int ligne = 0; ligne < hauteur; ++ligne) {
        disparityComputation(largeur, hauteur, image_L, image_R, ligne,
                             disparity, dispMax, dispMin);
    }

    clock_t time2 = clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    std::cout << "Calcule fait en: " << (elapsed) << " s" << std::endl;

    //! Display the depth map.
    Imagine::Image<Imagine::Color, 2> depth(largeur, hauteur);
    depth = disparityToDepth(disparity, dispMax, dispMin, largeur, hauteur);
    display(depth, Imagine::IntPoint2(largeur * 2, 0));

    Imagine::endGraphics();

    return 0;
}