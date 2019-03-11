#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <sys/timeb.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <locale>
#include <codecvt>
#include <bitset>
#include <mbctype.h>

//tesseract OCR Including file
#include "allheaders.h"
#include "baseapi.h"

//#include <comdef.h>

using namespace std;
using namespace tesseract;
using namespace cv;

#pragma comment(lib, "liblept171")
#pragma comment(lib, "libtesseract304")

Pix *image;// input image with leptonica library

//for checking Elapssd time.(milliseconds)
int getMilli()
{
	timeb t;
	ftime(&t);
	int n = t.millitm + (t.time & 0xfffff) * 1000;
	return n;
}
int getMilliSpan(int ntimestart)
{
	int nSpan = getMilli() - ntimestart;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
	
}

void showImage(char* imagepath) //function that shows the image for OCR to user
{
	IplImage *img = cvLoadImage(imagepath, 1);
	cvShowImage("Image for OCR", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
}

char* Image_Processing_3(char* imagepath)//grayscale + blur + threshold
{
	/* Load input image */
	cv::Mat img = cv::imread(imagepath);
	
	if (img.empty())
	{
		std::cout << "Failed to open image: " << img << std::endl;
		exit(1);
	}

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//specify the compression technique
	//vector that stores the compression parameters of the image
	compression_params.push_back(100); //specify the compression quality

	/*Image Preprocessing 20160210*/
	//1. RGB to GRAY
	//2. Smooth filter to simple noise elimination
	//3. Binary threshold(Threshold image to get binary image)
	//4. Morphologic filter(erode and dilate to eliminate noise)

	char* newImagePath;
	
	/* Convert to grayscale */
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	newImagePath = "./process_image_3/Gray_Image.jpg";
	cv::imwrite(newImagePath, gray, compression_params); 

	/* Bilateral filter helps to improve the segmentation process */
	cv::Mat blur;
	//cv::bilateralFilter(gray, blur, 99, 198, 50);//It increase the accuracy but, it takes long time
	cv::bilateralFilter(gray, blur, 49, 98, 25);
	newImagePath = "./process_image_3/Blur_Image.jpg";
	cv::imwrite(newImagePath, blur, compression_params);
	//cv::imshow("Filter", blur);
	//if I use Gussian blur, what happend?


	/* Threshold to binarize the image */
	cv::Mat thres;
	cv::adaptiveThreshold(blur, thres, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 2); //15, 2
	newImagePath = "./process_image_3/Threshold_Image.jpg";
	cv::imwrite(newImagePath, thres, compression_params);
	//cv::imshow("Threshold", thres);

	cv::waitKey(0);
	return newImagePath;
	/*end of image processing*/
}




char* Image_Processing_2(char* imagepath)//grayscale + threshold
{
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//specify the compression technique
	//vector that stores the compression parameters of the image
	compression_params.push_back(100); //specify the compression quality

	Mat im_gray = imread(imagepath, CV_LOAD_IMAGE_GRAYSCALE);
	
	Mat im_rgb = imread(imagepath);
	
	cvtColor(im_rgb, im_gray, CV_RGB2GRAY);

	Mat img_bw = im_gray > 115;
	cv::threshold(im_gray, img_bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	

	imwrite("./process_image_2/newprocessimg.jpg", img_bw, compression_params);
	return "./process_image_2/newprocessimg.jpg";
}


char* Image_Processing(char* imagepath) //just use grayscale function of OpenCV
{
	/* Load input image */
	cv::Mat img = cv::imread(imagepath);
	if (img.empty())
	{
		std::cout << "Failed to open image: " << img << std::endl;
		exit(1);
	}

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//specify the compression technique
	//vector that stores the compression parameters of the image
	compression_params.push_back(100); //specify the compression quality

	char* newImagePath;
	/* Convert to grayscale */
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	newImagePath = "./process_image/Gray_Image.jpg";
	cv::imwrite(newImagePath, gray, compression_params);
	
	cv::waitKey(0);
	return "./process_image/Gray_Image.jpg";
	/*end of image processing*/
}
std::wstring readFile(const char* filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

void FindKeyword() //Not working
{	//outText is utf-8 text.
	const int oldMbcp = _getmbcp();
	_setmbcp(936);
	const std::locale locale("Chinese_China.936");
	_setmbcp(oldMbcp);

	std::wstring outText = readFile("./a.txt");
	
	bool exist = FALSE;

	cout << "\n\n <Find the keyword whether it is in that image or not \n ";
	cout << "Enter word to search for: ";
	wstring search;
	wcin >> search;

	size_t pos = outText.find(search);
	if (pos != string::npos) // string::npos is returned if string is not found
	{
			cout << "Found!" << endl;
			exist = true;
	}
	else if (!exist)
		cout << "Not Found" << endl;
}




//use sample code
int main(int argc, CHAR* argv[])
{
	int process_again;
	cout << "<Select the menu> \n";
	cout << "1. This is first OCR process for this image(Default)\n";
	cout << "2. If the result of '1' is bad, Apply Grayscale\n"; // Image_Processing()
	cout << "3. If Background is a little bit complex, Apply Grayscale+Threshold\n";//Image_Processing_2()
	cout << "4. If Background is complex, Apply Grayscale+Blur+Threshold\n";//Image_Processing_3() 2016-02-10 version more accuracy
	cout << "Select : ";
	cin >> process_again;

	char* imagepath = "./image/simple10.jpg"; // change the path of Image that you want to do OCR.
	

	int start = getMilli();
	printf("\n \n The program starts \n");

	
	/*image processing to increase accuracy*/

	char *outText; //the result of OCR
	//const char *path = "C:/Tesseract-Build/tesseract-ocr/tessdata";

	TessBaseAPI *api = new TessBaseAPI();

	printf("initialize tesseract-ocr with chinese \n");
	
	//there is error : read_params_file : parameter not found when I use chinese language traineddata.
	//I solved the error by VietOCR.net. In that program, I downloaded chinese trainneddata file, and move it to my tessdata directory. It works!!
	
	if (api->Init(NULL, "chi_sim"))//Init(path, "chi_sim", tesseract::OEM_DEFAULT))
	{
		printf("Could not initialize tesseract\n");
		exit(1);
	}
	
	// Open input image with leptonica library
	if(process_again==1)
		image= pixRead(imagepath);//Only use tesseract library.
	else if (process_again==2)
		image = pixRead(Image_Processing(imagepath)); //use opencv image processing
	else if (process_again ==3)
		image = pixRead(Image_Processing_2(imagepath)); //use opencv image processing_2
	else if (process_again == 4)
	{
		image = pixRead(Image_Processing_3(imagepath)); //use opencv image processing_3
	}
	else if (process_again != 1 | 2 | 3 | 4)
	{
		cout << "\nYou select wrong number. Please restart";
		return(-1);
	}
	
	if (image == NULL)
	{
		printf("no file");
		exit(1);
	}

	api->SetImage(image);

	// Get OCR result
	outText = api->GetUTF8Text();
	printf("OCR output \n%s", outText);

	//store texts to txt file.
	FILE *fp = fopen("./a.txt", "w");
	fputs(outText, fp);
	fclose(fp);

	int milliElapsed = getMilliSpan(start);
	printf("\n\n consumed time(miiliseconds) is %d \n", milliElapsed);

	api->End();
//	delete[] outText; //runtime err //there is error : block type is valid(phead-nblockuse)
//	pixDestroy(&image);// when I use it with opencv, there is runtime error. I don't know why.

	//show the image for OCR to user
	showImage(imagepath);
	system("notepad.exe \"./a.txt\"");

	//FindKeyword();

	return 0;
}