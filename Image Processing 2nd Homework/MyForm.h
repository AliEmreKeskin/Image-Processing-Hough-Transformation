#pragma once
#include <Windows.h>
#include <atlstr.h>
#include "Imge_BMP.h"

namespace ImageProcessing2ndHomework {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		LPCTSTR input, output;
		int Width, Height;
		int raw_width, raw_height;
		long Size;
		BYTE* Buffer;
		BYTE* Raw_BMP;
		BYTE* Raw_Intensity;
		BYTE* Sobel_Ver;
		BYTE* Sobel_Hor;
		BYTE* Edge_Image;
		BYTE* Non_Max_Supp_Vertical;
		BYTE* Non_Max_Supp_Horizontal;
		BYTE* Non_Max_Supp;
		BYTE* HT_Vertical;
		BYTE* HT_Horizontal;
		BYTE* HT;
		BYTE* Gauss;
		BYTE* Hough;
		int hWidth;
		int hHeight;
		BYTE* Lined_Intensity;


	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ýntensityToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  edgeDetectionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sobelToolStripMenuItem;		 
	private: System::Windows::Forms::ToolStripMenuItem^  hysteresisThresholdToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gaussianSmoothingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  houghSpaceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  histogramEqualizationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  houghTranslationToolStripMenuItem;
	private: System::Windows::Forms::VScrollBar^  vScrollBar1;

	private: System::Windows::Forms::ToolStripMenuItem^  nonMaximumSuppressionToolStripMenuItem;

			 


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ýntensityToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gaussianSmoothingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->histogramEqualizationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->edgeDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sobelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nonMaximumSuppressionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hysteresisThresholdToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->houghSpaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->houghTranslationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->fileToolStripMenuItem,
					this->editToolStripMenuItem, this->edgeDetectionToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(284, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::openToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->ýntensityToolStripMenuItem,
					this->gaussianSmoothingToolStripMenuItem, this->histogramEqualizationToolStripMenuItem
			});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// ýntensityToolStripMenuItem
			// 
			this->ýntensityToolStripMenuItem->Name = L"ýntensityToolStripMenuItem";
			this->ýntensityToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->ýntensityToolStripMenuItem->Text = L"BMP to Intensity";
			this->ýntensityToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ýntensityToolStripMenuItem_Click);
			// 
			// gaussianSmoothingToolStripMenuItem
			// 
			this->gaussianSmoothingToolStripMenuItem->Name = L"gaussianSmoothingToolStripMenuItem";
			this->gaussianSmoothingToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->gaussianSmoothingToolStripMenuItem->Text = L"Gaussian Smoothing";
			this->gaussianSmoothingToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::gaussianSmoothingToolStripMenuItem_Click);
			// 
			// histogramEqualizationToolStripMenuItem
			// 
			this->histogramEqualizationToolStripMenuItem->Name = L"histogramEqualizationToolStripMenuItem";
			this->histogramEqualizationToolStripMenuItem->Size = System::Drawing::Size(197, 22);
			this->histogramEqualizationToolStripMenuItem->Text = L"Histogram Equalization";
			this->histogramEqualizationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::histogramEqualizationToolStripMenuItem_Click_1);
			// 
			// edgeDetectionToolStripMenuItem
			// 
			this->edgeDetectionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->sobelToolStripMenuItem,
					this->nonMaximumSuppressionToolStripMenuItem, this->hysteresisThresholdToolStripMenuItem, this->houghSpaceToolStripMenuItem,
					this->houghTranslationToolStripMenuItem
			});
			this->edgeDetectionToolStripMenuItem->Name = L"edgeDetectionToolStripMenuItem";
			this->edgeDetectionToolStripMenuItem->Size = System::Drawing::Size(99, 20);
			this->edgeDetectionToolStripMenuItem->Text = L"Edge Detection";
			// 
			// sobelToolStripMenuItem
			// 
			this->sobelToolStripMenuItem->Name = L"sobelToolStripMenuItem";
			this->sobelToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->sobelToolStripMenuItem->Text = L"Sobel";
			this->sobelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::sobelToolStripMenuItem_Click);
			// 
			// nonMaximumSuppressionToolStripMenuItem
			// 
			this->nonMaximumSuppressionToolStripMenuItem->Name = L"nonMaximumSuppressionToolStripMenuItem";
			this->nonMaximumSuppressionToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->nonMaximumSuppressionToolStripMenuItem->Text = L"Non-Maximum Suppression";
			this->nonMaximumSuppressionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::nonMaximumSuppressionToolStripMenuItem_Click);
			// 
			// hysteresisThresholdToolStripMenuItem
			// 
			this->hysteresisThresholdToolStripMenuItem->Name = L"hysteresisThresholdToolStripMenuItem";
			this->hysteresisThresholdToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->hysteresisThresholdToolStripMenuItem->Text = L"Hysteresis Threshold";
			this->hysteresisThresholdToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::hysteresisThresholdToolStripMenuItem_Click);
			// 
			// houghSpaceToolStripMenuItem
			// 
			this->houghSpaceToolStripMenuItem->Name = L"houghSpaceToolStripMenuItem";
			this->houghSpaceToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->houghSpaceToolStripMenuItem->Text = L"Hough Space";
			this->houghSpaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::houghSpaceToolStripMenuItem_Click);
			// 
			// houghTranslationToolStripMenuItem
			// 
			this->houghTranslationToolStripMenuItem->Name = L"houghTranslationToolStripMenuItem";
			this->houghTranslationToolStripMenuItem->Size = System::Drawing::Size(223, 22);
			this->houghTranslationToolStripMenuItem->Text = L"Hough Translation";
			this->houghTranslationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::houghTranslationToolStripMenuItem_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 27);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(100, 50);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Location = System::Drawing::Point(267, 172);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(17, 80);
			this->vScrollBar1->TabIndex = 2;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->vScrollBar1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion



	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			pictureBox1->ImageLocation = openFileDialog1->FileName;
			CString str;
			str = openFileDialog1->FileName;
			input = (LPCTSTR)str;
			Raw_BMP = LoadBMP(Width, Height, Size, input);
			raw_height = Height;
			raw_width = Width;
			Buffer = LoadBMP(Width, Height, Size, input);
		}
	}
	private: System::Void ýntensityToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Raw_Intensity = ConvertBMPToIntensity(Buffer, Width, Height);
		Draw_Line_Polar_2(Raw_Intensity, Width, Height, 34, 33);
		// picture
		{
			Bitmap^ surface = gcnew Bitmap(Width, Height);
			pictureBox1->Image = surface;
			Color c;
			int temp;
			for (unsigned int row = 0; row < Height; row++) {
				for (unsigned int column = 0; column < Width; column++) {
					temp = row * Width + column;
					c = Color::FromArgb(Raw_Intensity[temp], Raw_Intensity[temp], Raw_Intensity[temp]);
					surface->SetPixel(column, row, c);
				}
			}
		}
	}
	private: System::Void sobelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Sobel_Ver = Sobel_Vertical_Func(Gauss, Width, Height);
		Sobel_Hor = Sobel_Horizontal_Func(Gauss, Width, Height);
		Edge_Image = Sobel(Raw_Intensity, Width, Height, Sobel_Ver, Sobel_Hor);
		Width = Width - 2;
		Height = Height - 2;

		// picture
		{
			Bitmap^ surface = gcnew Bitmap(Width, Height);
			pictureBox1->Image = surface;
			Color c;
			int temp;
			for (unsigned int row = 0; row < Height; row++) {
				for (unsigned int column = 0; column < Width; column++) {
					temp = row * Width + column;
					c = Color::FromArgb(Edge_Image[temp], Edge_Image[temp], Edge_Image[temp]);
					surface->SetPixel(column, row, c);
				}
			}
		}
	}
	private: System::Void nonMaximumSuppressionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Non_Max_Supp_Horizontal = NMS_Horizontal(Edge_Image, Width, Height);
		Non_Max_Supp_Vertical = NMS_Vertical(Edge_Image, Width, Height);
		Non_Max_Supp = NMS(Edge_Image, Width, Height, Non_Max_Supp_Vertical, Non_Max_Supp_Horizontal);
		// picture
		{
			Bitmap^ surface = gcnew Bitmap(Width, Height);
			pictureBox1->Image = surface;
			Color c;
			int temp;
			for (unsigned int row = 0; row < Height; row++) {
				for (unsigned int column = 0; column < Width; column++) {
					temp = row * Width + column;
					c = Color::FromArgb(Non_Max_Supp[temp], Non_Max_Supp[temp], Non_Max_Supp[temp]);
					surface->SetPixel(column, row, c);
				}
			}
		}
	}
	private: System::Void hysteresisThresholdToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		HT_Vertical = Hysteresis_Threshold_Vertical(Non_Max_Supp_Horizontal, Width, Height,15,1);
		HT_Horizontal = Hysteresis_Threshold_Horizontal(Non_Max_Supp_Vertical, Width, Height,15,1);
		HT = Hysteresis_Threshold(Non_Max_Supp_Vertical, Non_Max_Supp_Horizontal, Width, Height, HT_Vertical, HT_Horizontal);
		// picture
		{
			Bitmap^ surface = gcnew Bitmap(Width, Height);
			pictureBox1->Image = surface;
			Color c;
			int temp;
			for (unsigned int row = 0; row < Height; row++) {
				for (unsigned int column = 0; column < Width; column++) {
					temp = row * Width + column;
					c = Color::FromArgb(HT[temp], HT[temp], HT[temp]);
					surface->SetPixel(column, row, c);
				}
			}
		}
	}
	private: System::Void gaussianSmoothingToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Gauss = Gaussian_Smoothing(Raw_Intensity, Width, Height);
		Width -= 4;
		Height -= 4;


		// picture
		{
			Bitmap^ surface = gcnew Bitmap(Width, Height);
			pictureBox1->Image = surface;
			Color c;
			int temp;
			for (unsigned int row = 0; row < Height; row++) {
				for (unsigned int column = 0; column < Width; column++) {
					temp = row * Width + column;
					c = Color::FromArgb(Gauss[temp], Gauss[temp], Gauss[temp]);
					surface->SetPixel(column, row, c);
				}
			}
		}
	}
private: System::Void houghSpaceToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	Hough = Hough_Space(HT, Sobel_Ver, Sobel_Hor, Width, Height);
	hWidth = 360;
	hHeight = sqrt(Width*Width + Height * Height);

	

	// picture
	{
		Bitmap^ surface = gcnew Bitmap(hWidth, hHeight);
		pictureBox1->Image = surface;
		Color c;
		int temp;
		for (unsigned int row = 0; row < hHeight; row++) {
			for (unsigned int column = 0; column < hWidth; column++) {
				temp = row * hWidth + column;
				c = Color::FromArgb(Hough[temp], Hough[temp], Hough[temp]);
				surface->SetPixel(column, row, c);
			}
		}
	}
}

private: System::Void histogramEqualizationToolStripMenuItem_Click_1(System::Object^  sender, System::EventArgs^  e) {
	Hough = Histogram_Equalization(Hough, hWidth, hHeight);

	// picture
	{
		Bitmap^ surface = gcnew Bitmap(hWidth, hHeight);
		pictureBox1->Image = surface;
		Color c;
		int temp;
		for (unsigned int row = 0; row < hHeight; row++) {
			for (unsigned int column = 0; column < hWidth; column++) {
				temp = row * hWidth + column;
				c = Color::FromArgb(Hough[temp], Hough[temp], Hough[temp]);
				surface->SetPixel(column, row, c);
			}
		}
	}
}
private: System::Void houghTranslationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	Lined_Intensity = Hough_Translation(Raw_Intensity, raw_width, raw_height, Hough, hWidth, hHeight);

	// picture
	{
		Bitmap^ surface = gcnew Bitmap(raw_width, raw_height);
		pictureBox1->Image = surface;
		Color c;
		int temp;
		for (unsigned int row = 0; row < raw_height; row++) {
			for (unsigned int column = 0; column < raw_width; column++) {
				temp = row * raw_width + column;
				c = Color::FromArgb(Lined_Intensity[temp], Lined_Intensity[temp], Lined_Intensity[temp]);
				surface->SetPixel(column, row, c);
			}
		}
	}
}
};
}
