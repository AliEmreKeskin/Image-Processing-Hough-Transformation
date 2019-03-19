#include <windows.h>
#include <math.h>
#define PI 3.14159256

BYTE* LoadBMP(int% width, int% height, long% size, LPCTSTR bmpfile)
{
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL; // coudn't open file

					 // read file header
	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	//read bitmap info
	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB') {
		CloseHandle(file);
		return NULL;
	}
	// get image measurements
	width = bmpinfo.biWidth;
	height = abs(bmpinfo.biHeight);

	// check if bmp is uncompressed
	if (bmpinfo.biCompression != BI_RGB) {
		CloseHandle(file);
		return NULL;
	}
	// check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24) {
		CloseHandle(file);
		return NULL;
	}

	// create buffer to hold the data
	size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, size, &bytesread, NULL) == false) {
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}
	// everything successful here: close file and return buffer
	CloseHandle(file);

	return Buffer;
}//LOADPMB

bool SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile)
{
	// declare bmp structures 
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	// andinitialize them to zero
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	// fill the fileheader with data
	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + paddedsize;
	bmfh.bfOffBits = 0x36;		// number of bytes to start of bitmap bits

								// fill the infoheader

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;			// we only have one bitplane
	info.biBitCount = 24;		// RGB mode is 24 bits
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;		// can be 0 for 24 bit images
	info.biXPelsPerMeter = 0x0ec4;     // paint and PSP use this values
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;			// we are in RGB mode and have no palette
	info.biClrImportant = 0;    // all colors are important

								// now we open the file to write to
	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		CloseHandle(file);
		return false;
	}
	// write file header
	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof(BITMAPFILEHEADER), &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}
	// write infoheader
	if (WriteFile(file, &info, sizeof(BITMAPINFOHEADER), &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}
	// write image data
	if (WriteFile(file, Buffer, paddedsize, &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}

	// and close file
	CloseHandle(file);

	return true;
} // SaveBMP

BYTE* ConvertBMPToIntensity(BYTE* Buffer, int width, int height)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// find the number of padding bytes

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = new BYTE[width*height];

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++) {
			newpos = row * width + column;
			bufpos = (height - row - 1) * psw + column * 3;
			newbuf[newpos] = BYTE(0.11*Buffer[bufpos + 2] + 0.59*Buffer[bufpos + 1] + 0.3*Buffer[bufpos]);
		}

	return newbuf;
}//ConvetBMPToIntensity

BYTE* ConvertIntensityToBMP(BYTE* Buffer, int width, int height, long* newsize)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;
	// we can already store the size of the new padded buffer
	*newsize = height * psw;

	// and create new buffer
	BYTE* newbuf = new BYTE[*newsize];

	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset(newbuf, 0, *newsize);

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++) {
			bufpos = row * width + column;     // position in original buffer
			newpos = (height - row - 1) * psw + column * 3;           // position in padded buffer
			newbuf[newpos] = Buffer[bufpos];       //  blue
			newbuf[newpos + 1] = Buffer[bufpos];   //  green
			newbuf[newpos + 2] = Buffer[bufpos];   //  red
		}

	return newbuf;
} //ConvertIntensityToBMP

BYTE* Sobel_Vertical_Func(BYTE* Raw_Intensity, int width, int height) {
	int vertical[9] = { -1,-2,-1,0,0,0,1,2,1 };
	BYTE* newbuf = new BYTE[(width - 2)*(height - 2)];
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			int pos = (i - 1)*width + (j - 1);
			int temp = 0;
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					temp += Raw_Intensity[pos + k * width + l] * vertical[k * 3 + l];
					newbuf[(i - 1)*(width - 2) + j - 1] = abs(temp / 6);
				}
			}
		}
	}
	return newbuf;
}

BYTE* Sobel_Horizontal_Func(BYTE* Raw_Intensity, int width, int height) {
	int horizontal[9] = { -1,0,1,-2,0,2,-1,0,1 };
	BYTE* newbuf = new BYTE[(width - 2)*(height - 2)];
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			int pos = (i - 1)*width + (j - 1);
			int temp = 0;
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					temp += Raw_Intensity[pos + k * width + l] * horizontal[k * 3 + l];
					newbuf[(i - 1)*(width - 2) + j - 1] = abs(temp / 6);
				}
			}
		}
	}
	return newbuf;
}

BYTE* Sobel(BYTE* Raw_Intensity, int width, int height, BYTE* vertical, BYTE* horizontal) {
	//vertical = Sobel_Vertical(Raw_Intensity, width, height);
	//horizontal = Sobel_Horizontal(Raw_Intensity, width, height);
	BYTE* newbuf = new BYTE[(width - 2)*(height - 2)];
	for (int i = 0; i < (width - 2)*(height - 2); i++) {
		newbuf[i] = vertical[i] + horizontal[i];
	}
	return newbuf;
}

BYTE* NMS_Vertical(BYTE* Edge_Image, int width, int height) {
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = Edge_Image[i];
	}
	for (int i = 0; i < width*height; i++) {
		//upper border
		if (i < width) {
			if (Edge_Image[i] < Edge_Image[i + width]) {
				newbuf[i] = 0;
			}
		}
		//lower border
		else if (i > (height - 1)*width - 1) {
			if (Edge_Image[i] < Edge_Image[i - width]) {
				newbuf[i] = 0;
			}
		}
		//non-border
		else if (Edge_Image[i] <= Edge_Image[i + width] || Edge_Image[i] < Edge_Image[i - width]) {
			newbuf[i] = 0;
		}
	}
	return newbuf;
}

BYTE* NMS_Horizontal(BYTE* Edge_Image, int width, int height) {
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = Edge_Image[i];
	}
	for (int i = 0; i < width*height; i++) {
		//left border
		if (i%width == 0) {
			if (Edge_Image[i] < Edge_Image[i + 1]) {
				newbuf[i] = 0;
			}
		}
		//right border
		else if (i%width == width - 1) {
			if (Edge_Image[i] < Edge_Image[i - 1]) {
				newbuf[i] = 0;
			}
		}
		//non-border
		else if (Edge_Image[i] <= Edge_Image[i + 1] || Edge_Image[i] < Edge_Image[i - 1]) {
			newbuf[i] = 0;
		}
	}
	return newbuf;
}

BYTE* NMS(BYTE* Edge_Image, int width, int height, BYTE* vertical, BYTE* horizontal) {
	//vertical = NMS_Vertical(Edge_Image, width, height);
	//horizontal = NMS_Horizontal(Edge_Image, width, height);
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = vertical[i] + horizontal[i];
	}
	return newbuf;
}

BYTE* Hysteresis_Threshold_Horizontal(BYTE* NMS_Vertical, int width, int height,int upper,int lower) {
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = 0;
	}
	for (int i = 0; i < width*height; i++) {
		if (NMS_Vertical[i] >= upper) {
			newbuf[i] = 255;
		}
		else if (NMS_Vertical[i] < lower) {
			newbuf[i] = 0;
		}
		else {
			//left border
			if (i%width == 0) {
				if (NMS_Vertical[i + 1] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
			//right border
			else if (i%width == width - 1) {
				if (NMS_Vertical[i - 1] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
			//non-border
			else {
				if (NMS_Vertical[i + 1] > upper || NMS_Vertical[i - 1] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
		}
	}
	return newbuf;
}

BYTE* Hysteresis_Threshold_Vertical(BYTE* NMS_Horizontal, int width, int height,int upper,int lower) {
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = 0;
	}
	for (int i = 0; i < width*height; i++) {
		if (NMS_Horizontal[i] >= upper) {
			newbuf[i] = 255;
		}
		else if (NMS_Horizontal[i] < lower) {
			newbuf[i] = 0;
		}
		else {
			//upper border
			if (i<width) {
				if (NMS_Horizontal[i + width] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
			//lower border
			else if (i >(height - 1)*width - 1) {
				if (NMS_Horizontal[i - width] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
			//non-border
			else {
				if (NMS_Horizontal[i + width] > upper || NMS_Horizontal[i - width] > upper) {
					newbuf[i] = 255;
				}
				else {
					newbuf[i] = 0;
				}
			}
		}
	}
	return newbuf;
}

BYTE* Hysteresis_Threshold(BYTE* NMS_Vertical, BYTE* NMS_Horizontal, int width, int height, BYTE* HT_Vertical, BYTE* HT_Horizontal) {
	//HT_Vertical = Hysteresis_Threshold_Vertical(NMS_Horizontal, width, height);
	//HT_Horizontal = Hysteresis_Threshold_Horizontal(NMS_Vertical, width, height);
	BYTE* newbuf = new BYTE[width*height];
	for (int i = 0; i < width*height; i++) {
		newbuf[i] = HT_Vertical[i] + HT_Horizontal[i];
	}
	return newbuf;
}

BYTE* Gaussian_Smoothing(BYTE* Raw_Intensity, int width, int height) {
	int gauss[25] = { 1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,4,16,26,16,4,1,4,7,4,1 };
	BYTE* newbuf = new BYTE[(width - 4)*(height - 4)];
	for (int i = 2; i < height - 2; i++) {
		for (int j = 2; j < width - 2; j++) {
			int pos = (i - 2)*width + (j - 2);
			int temp = 0;
			for (int k = 0; k < 5; k++) {
				for (int l = 0; l < 5; l++) {
					temp += Raw_Intensity[pos + k * width + l] * gauss[k * 5 + l];
					newbuf[(i - 2)*(width - 4) + j - 2] = temp/273;
				}
			}
		}
	}



	return newbuf;
}

BYTE* Hough_Space(BYTE* Canny_Image, BYTE* Sobel_Vertical, BYTE* Sobel_Horizontal, int Width, int Height) {
	int size = 360 * (sqrt(Width*Width + Height * Height));
	BYTE* hough = new BYTE[size];
	for (int i = 0; i < size; i++) {
		hough[i] = 0;
	}
	int theta;
	int d;
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			if (Canny_Image[i*Width+j] == 255) {
				if (Sobel_Vertical[i*Width + j] == 0) {
					theta = 90;
				}
				else {
					theta = atan(Sobel_Horizontal[i*Width + j] / Sobel_Vertical[i*Width + j]) * 180 / PI;
				}
				d = i * sin(theta*PI / 180) + j * cos(theta*PI / 180);
				hough[d * 360 + theta+180]=255;
			}
		}
	}
	return hough;
}

BYTE* Histogram_Equalization(BYTE* Raw_Intensity, int Width, int Height) {
	//histogram
	int* Histogram = new int[256];
	for (int i = 0; i < 256; i++) {
		Histogram[i] = 0;
	}
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			Histogram[Raw_Intensity[i*Width + j]]++;
		}
	}
	//equalization
	int* newValues = new int[256];
	float Sum = 0.0;
	float currentSum = 0.0;
	//calculate sum of histogram values
	for (int i = 0; i < 256; i++) {
		Sum += Histogram[i];
	}
	//produce equalized new values
	for (int i = 0; i < 256; i++) {
		currentSum += Histogram[i];
		newValues[i] = currentSum / Sum * 255;
	}
	//replace equalized values with not equalized old values
	BYTE* newbuf = new BYTE[Width*Height];
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			newbuf[i*Width + j] = newValues[Raw_Intensity[i*Width + j]];
		}
	}
	return newbuf;
}

void Draw_Line_Polar(BYTE* image, int w, int h, int r, int q) {
	double a, b;
	int x, y;
	x = r * cos(q*PI / 180);
	y = r * sin(q*PI / 180);
	if (y == 0) {
		a = 1;
	}
	else {
		a = x / y;
	}
	b = y - a * x;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (0-i+1 > a * j + b && 0-i-1 < a * j + b) {
				image[i*w + j] = 255;
			}
		}
	}
}

void Draw_Line_Polar_2(BYTE* image, int w, int h, int r, int q) {
	q = 180 - q;
	double a, b;
	int x0, y0;
	x0 = r * cos((q - 90)*PI / 180);
	y0 = r * sin((q - 90)*PI / 180);
	a = tan(q * PI / 180);
	//a = (0 - x0) / y0;
	b = y0 - a * x0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (i + 2 > a * j + b && i - 2 < a * j + b) {
				image[i*w + j] = 255;
			}
		}
	}
}

BYTE* Hough_Translation(BYTE* Raw_Intensity, int Raw_Width, int Raw_Height, BYTE* Hough, int Hough_Width, int Hough_Height) {
	BYTE* newbuf = new BYTE[Raw_Width*Raw_Height];
	for (int i = 0; i < Raw_Width*Raw_Height; i++)
	{
		newbuf[i] = Raw_Intensity[i];
	}
	for (int r = 0; r < Hough_Height; r++) {
		for (int q = 0; q < Hough_Width; q++) {
			if (Hough[r * 360 + q] != 0) {
				Draw_Line_Polar_2(newbuf, Raw_Width, Raw_Height, r, q);
				//Draw_Line_Polar_2(newbuf, Raw_Width, Raw_Height, 100, 60);
			}
		}
	}


	return newbuf;
}