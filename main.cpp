#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

#define VERDE ((0 << 16) | (255 << 8) | 0)

int c;

int *colorsTxt = new int[256 * 256 * 256];

void loadFile(string file)
{
    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        colorsTxt[i] = -1; // Aqui seta tudo pra -1 o vetor
    }
    string line;
    ifstream inFile(file.c_str(), ios::in);
    if (inFile)
    {
        while (getline(inFile, line))
        {
            //cout << line << '\n';
            istringstream actualColor(line);
            actualColor >> c;
            //cout << colorsTxt[c] << endl;
            //colorsTxt[c] = VERDE;
            colorsTxt[c] = 1; // Aqui seta pra 1 os valores que forem verdes no vetor
            //cout << colorsTxt[c] << endl;

            actualColor.str("");
        }
        inFile.close();
    }
    else
        cout << "could not open file" << endl;
}

void clustering(Mat img)
{
    int R, G, B;
    int col;
    int aux=0; // Variável auxiliar para contar os 5 pixel seguidos de verde
    for (int x = 0; x < img.cols; x++)
    {
        for (int y = 0; y < img.rows; y++)        
        {
            Vec3b color = img.at<Vec3b>(y, x);
            B = (int)color.val[0];
            G = (int)color.val[1];
            R = (int)color.val[2];
            col = (B << 16) | (G << 8) | R;

            if (colorsTxt[col] != -1)
            {
                //Se o pixel que tá sendo escolhido, nao for verde, terá valor -1 no vetor
                //Caso isso ocorra, pintar o pixel de verde
                aux++;
                color.val[0] = 0;
                color.val[1] = 0;
                color.val[2] = 255;
                if(aux==5)
                {    
                    
                    aux=0;
                    break;
                    //
                    // é pq aqui ele continua
                    //break; //Gerou nao kkk
                }
            }
            img.at<Vec3b>(y, x) = color;
        }
        x = x+4;
    }
    imshow("Clusterized", img);
}


int  main(int argc, char **argv)
{
    Mat img_rgb;
	loadFile("../clustering.txt");
    Mat img;
    img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    imshow("Imagem original", img);
    int j=0;
	for (int i = 0; i < 256 * 256 * 256; i++)
    {
        if(colorsTxt[i]==1)
        {
            j++;
        }
    }
    printf("%d",j);
    img_rgb = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    clustering(img_rgb);
    
    //resize(image, image, Size(700,700));
    waitKey(0);

}
