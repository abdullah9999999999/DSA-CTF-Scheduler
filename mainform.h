#pragma once
#include "teams.h" // Include the header file for the teams form
#include"schedule.h"
#include"help.h"
namespace DSP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for mainform
	/// </summary>
	public ref class mainform : public System::Windows::Forms::Form
	{
	public : Point mouseLocation;
	public:
		mainform(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~mainform()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::Button^ teams_button;
	private: System::Windows::Forms::Button^ schedule_button;

	private: System::Windows::Forms::Button^ help_button;
	private: System::Windows::Forms::Button^ exit_button;
	private: System::Windows::Forms::PictureBox^ pictureBox1;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(mainform::typeid));
			this->teams_button = (gcnew System::Windows::Forms::Button());
			this->schedule_button = (gcnew System::Windows::Forms::Button());
			this->help_button = (gcnew System::Windows::Forms::Button());
			this->exit_button = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// teams_button
			// 
			this->teams_button->BackColor = System::Drawing::Color::White;
			this->teams_button->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->teams_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->teams_button->Font = (gcnew System::Drawing::Font(L"Impact", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->teams_button->ForeColor = System::Drawing::Color::Black;
			this->teams_button->Location = System::Drawing::Point(133, 434);
			this->teams_button->Name = L"teams_button";
			this->teams_button->Size = System::Drawing::Size(210, 45);
			this->teams_button->TabIndex = 1;
			this->teams_button->Text = L"TEAMS";
			this->teams_button->UseVisualStyleBackColor = false;
			this->teams_button->Click += gcnew System::EventHandler(this, &mainform::teams_button_Click);
			// 
			// schedule_button
			// 
			this->schedule_button->BackColor = System::Drawing::Color::White;
			this->schedule_button->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->schedule_button->FlatAppearance->BorderSize = 2;
			this->schedule_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->schedule_button->Font = (gcnew System::Drawing::Font(L"Impact", 12));
			this->schedule_button->ForeColor = System::Drawing::Color::Black;
			this->schedule_button->Location = System::Drawing::Point(407, 434);
			this->schedule_button->Name = L"schedule_button";
			this->schedule_button->Size = System::Drawing::Size(210, 45);
			this->schedule_button->TabIndex = 2;
			this->schedule_button->Text = L"SCHEDULE";
			this->schedule_button->UseVisualStyleBackColor = false;
			this->schedule_button->Click += gcnew System::EventHandler(this, &mainform::schedule_button_Click);
			// 
			// help_button
			// 
			this->help_button->BackColor = System::Drawing::Color::White;
			this->help_button->FlatAppearance->BorderSize = 2;
			this->help_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->help_button->Font = (gcnew System::Drawing::Font(L"Impact", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->help_button->ForeColor = System::Drawing::Color::Black;
			this->help_button->Location = System::Drawing::Point(674, 434);
			this->help_button->Name = L"help_button";
			this->help_button->Size = System::Drawing::Size(210, 45);
			this->help_button->TabIndex = 4;
			this->help_button->Text = L"HELP";
			this->help_button->UseVisualStyleBackColor = false;
			this->help_button->Click += gcnew System::EventHandler(this, &mainform::help_button_Click);
			// 
			// exit_button
			// 
			this->exit_button->BackColor = System::Drawing::Color::Black;
			this->exit_button->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->exit_button->FlatAppearance->BorderSize = 2;
			this->exit_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->exit_button->Font = (gcnew System::Drawing::Font(L"Kabisat Demo Tall", 8, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->exit_button->ForeColor = System::Drawing::Color::White;
			this->exit_button->Location = System::Drawing::Point(407, 504);
			this->exit_button->Name = L"exit_button";
			this->exit_button->Size = System::Drawing::Size(210, 45);
			this->exit_button->TabIndex = 5;
			this->exit_button->Text = L"End";
			this->exit_button->UseVisualStyleBackColor = false;
			this->exit_button->Click += gcnew System::EventHandler(this, &mainform::exit_button_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1028, 594);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 6;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::mouse_Down);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::mouse_Move);
			// 
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1028, 594);
			this->Controls->Add(this->exit_button);
			this->Controls->Add(this->help_button);
			this->Controls->Add(this->schedule_button);
			this->Controls->Add(this->teams_button);
			this->Controls->Add(this->pictureBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"mainform";
			this->Text = L"mainform";
			this->Load += gcnew System::EventHandler(this, &mainform::mainform_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void teams_button_Click(System::Object^ sender, System::EventArgs^ e) {
		// Create an instance of the teams form and show it
		teams^ teamsForm = gcnew teams();
		teamsForm->ShowDialog(); // Show it as a modal dialog
	}
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void schedule_button_Click(System::Object^ sender, System::EventArgs^ e) {

	DSP::schedule^ scheduleForm = gcnew DSP::schedule(); // Create a new instance of the schedule form
	scheduleForm->ShowDialog(); // Show it as a modal dialog
}
private: System::Void mainform_Load(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void help_button_Click(System::Object^ sender, System::EventArgs^ e) {
	DSP::help^ helpForm = gcnew DSP::help(); // Create a new instance of the help form
	helpForm->ShowDialog(); // Show it as a modal dialog
	
}
private: System::Void exit_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit(); // Exit the application
}
private: System::Void mouse_Down(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		mouseLocation = Point(-e->X, -e->Y);
	}
private: System::Void mouse_Move(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (e->Button == Windows::Forms::MouseButtons::Left) {
		Point mousePose = Control::MousePosition;
		mousePose.Offset(mouseLocation.X, mouseLocation.Y);
		Location = mousePose;
	}
}
};
}