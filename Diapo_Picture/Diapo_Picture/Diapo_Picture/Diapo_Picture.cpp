#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <windows.h>

using namespace cv;
using namespace std;




int main(int argc, char** argv)
{

    VideoCapture cap(0); //capturer la vidéo de la webcam

    if (!cap.isOpened())  // si ce n'est pas le cas, quittez le programme
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }




    int iLowH = 170;
    int iHighH = 179;

    int iLowS = 150;
    int iHighS = 255;

    int iLowV = 60;
    int iHighV = 255;
    Mat frame2;


    int iLastX = -1;
    int iLastY = -1;


    

    //Capturer une image temporaire à partir de l'appareil photo
    Mat imgTmp;
    cap.read(imgTmp);
    vector<String> FilesImad;

    vector<Mat> images;
    vector<Mat> imagesComp; // imagesCompressees


    Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;


    glob("D:/imad/TDopencv/Diapo_Picture/rsc/*.jpg", FilesImad, false);
    //glob("C:/Imad/*.jpg", FilesImad, false); // à enlever, remets le premier


    int count = FilesImad.size();


    for (int i = 0; i < count; i++)
    {
        Mat image = imread(FilesImad[i]);
        images.push_back(image);
        imagesComp.push_back(image);
    }


    Mat fond(600, 1500, CV_8UC3, Scalar(0, 0, 0)); //fond de la galerie

    int widthComp = 75;     //taille des images compressées/vignettes
    int heigthComp = 100;


    //compression
    for (int i = 0; i < 20; i++)
    {
        resize(images[i], imagesComp[i], Size(widthComp, heigthComp), INTER_LINEAR);
    }

    

    //mise en place des vignettes dans le fond
    for (int i = 0; i < 100; i++)
    {
        for (int nrImage = 0; nrImage <20; nrImage++)
        {
            for (int j = 0; j < 75; j++)
            {
                fond.at<Vec3b>(500 + i, j + (75 * nrImage))[0] = imagesComp[nrImage].at<Vec3b>(i, j)[0];
                fond.at<Vec3b>(500 + i, j + (75 * nrImage))[1] = imagesComp[nrImage].at<Vec3b>(i, j)[1];
                fond.at<Vec3b>(500 + i, j + (75 * nrImage))[2] = imagesComp[nrImage].at<Vec3b>(i, j)[2];
            }
        }

    }
    



    int chosenImage; //image sélectionnée par IHM/caméra



    while (true)
    {


        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal); // lire une nouvelle image de la vidéo


        if (!bSuccess) // si ce n'est pas réussi, rompre la boucle
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convertir l'image capturée de BGR en HSV

        Mat imgThresholded, imgThresholded2, imgThresholded3;

        //  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        inRange(imgHSV, Scalar(0, 160, 160), Scalar(10, 255, 255), imgThresholded);
        inRange(imgHSV, Scalar(170, 160, 160), Scalar(180, 255, 255), imgThresholded2);
        // inRange(imgHSV, Scalar(94, 80, 2), Scalar(126, 255, 255), imgThresholded3);
        frame2 = imgThresholded | imgThresholded2;

        //morphological opening (removes small objects from the foreground)
        erode(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        Mat fond2 = fond.clone(); //on a une image affichée mais on garde le fond
        // pour effacer l'image affichée

        flip(imgOriginal, imgOriginal, 1);
        flip(frame2, frame2, 1);

        //Affichage du curseur
       // circle(fond2, Point(posX, posY), 10, Scalar(255, 0, 0), -1, 10, 0);

        //Calculer les moments de l'image seuillée
        Moments oMoments = moments(frame2);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;


        //calculate the position of the ball
        int posX = dM10 / dArea *2.1;
        int posY = dM01 / dArea * 1.24; //nécessaire de multiplier pour passer de la taille de la caméra à la taille du fond

        circle(fond2, Point(posX, posY), 10, Scalar(0, 0, 255), -1, 10, 0);
        
      

        
        // si l'on est dans la bordure des vignettes on peut changer d'image
        if (posY > 500)
        {
            chosenImage = (int)(posX / 75);

 


            for (int i = 0; i < images[chosenImage].rows; i++)
            {

                for (int j = 0; j < images[chosenImage].cols; j++)
                {
                    fond2.at<Vec3b>(i, j)[0] = images[chosenImage].at<Vec3b>(i, j)[0];
                    fond2.at<Vec3b>(i, j)[1] = images[chosenImage].at<Vec3b>(i, j)[1];
                    fond2.at<Vec3b>(i, j)[2] = images[chosenImage].at<Vec3b>(i, j)[2];
                }


            }
        }

        
        imshow("Thresholded Image", frame2); //show the thresholded image
        imgOriginal = imgOriginal + imgLines;

        
        
       imshow("Original", imgOriginal); //show the original image
        imshow("fond", fond2);

        waitKey(50);
    }


    return 0;
}