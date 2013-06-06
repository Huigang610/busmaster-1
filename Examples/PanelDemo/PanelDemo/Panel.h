#pragma once

/* Visual Studio includes */
#include <sstream>
#include <msclr\marshal_cppstd.h>

/* Project includes */
#include "BUSMASTER_Interface.h"
#include "Signal_Interaction.h"

using msclr::interop::marshal_as;

#pragma comment(lib, "ole32.lib")

namespace Test {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::CheckBox^  checkBox7;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ProgressBar^  progressBar3;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label3;
	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->progressBar3 = (gcnew System::Windows::Forms::ProgressBar());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(24, 125);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Faster";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(135, 125);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Slower";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(284, 129);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(44, 17);
			this->checkBox1->TabIndex = 2;
			this->checkBox1->Text = L"Left";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(69, 84);
			this->progressBar1->Maximum = 320;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->progressBar1->Size = System::Drawing::Size(100, 23);
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(99, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Speed";
			// 
			// label2
			// 
			this->label2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label2->Location = System::Drawing::Point(84, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 25);
			this->label2->TabIndex = 5;
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(357, 129);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(51, 17);
			this->checkBox2->TabIndex = 9;
			this->checkBox2->Text = L"Right";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox2_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(357, 175);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(47, 17);
			this->checkBox3->TabIndex = 11;
			this->checkBox3->Text = L"ABS";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox3_CheckedChanged_1);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Location = System::Drawing::Point(284, 175);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(47, 17);
			this->checkBox4->TabIndex = 10;
			this->checkBox4->Text = L"ESP";
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox4_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Location = System::Drawing::Point(357, 152);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(75, 17);
			this->checkBox5->TabIndex = 13;
			this->checkBox5->Text = L"HighBeam";
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox5_CheckedChanged);
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Location = System::Drawing::Point(284, 152);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(64, 17);
			this->checkBox6->TabIndex = 12;
			this->checkBox6->Text = L"Daylight";
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox6_CheckedChanged);
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Location = System::Drawing::Point(314, 218);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(54, 17);
			this->checkBox7->TabIndex = 14;
			this->checkBox7->Text = L"Brake";
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox7_CheckedChanged);
			// 
			// label5
			// 
			this->label5->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label5->Location = System::Drawing::Point(295, 45);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(66, 25);
			this->label5->TabIndex = 17;
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(310, 23);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(31, 13);
			this->label6->TabIndex = 16;
			this->label6->Text = L"RPM";
			// 
			// progressBar3
			// 
			this->progressBar3->Location = System::Drawing::Point(280, 84);
			this->progressBar3->Maximum = 8400;
			this->progressBar3->Name = L"progressBar3";
			this->progressBar3->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->progressBar3->Size = System::Drawing::Size(100, 23);
			this->progressBar3->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar3->TabIndex = 15;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(440, 385);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 19;
			this->button3->Text = L"Disconnect";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Visible = false;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(329, 385);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 18;
			this->button4->Text = L"Connecet";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(24, 385);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(41, 23);
			this->button5->TabIndex = 21;
			this->button5->Text = L"Off";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(165, 385);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(41, 23);
			this->button6->TabIndex = 22;
			this->button6->Text = L"On";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(24, 247);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(41, 23);
			this->button7->TabIndex = 23;
			this->button7->Text = L"P";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(71, 247);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(41, 23);
			this->button8->TabIndex = 24;
			this->button8->Text = L"N";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(118, 247);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(41, 23);
			this->button9->TabIndex = 25;
			this->button9->Text = L"D";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click);
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(165, 247);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(41, 23);
			this->button10->TabIndex = 26;
			this->button10->Text = L"R";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::button10_Click);
			// 
			// label8
			// 
			this->label8->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label8->Location = System::Drawing::Point(84, 210);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(66, 25);
			this->label8->TabIndex = 27;
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(156, 52);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(32, 13);
			this->label3->TabIndex = 28;
			this->label3->Text = L"mp/h";

			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Gainsboro;
			this->ClientSize = System::Drawing::Size(532, 433);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->progressBar3);
			this->Controls->Add(this->checkBox7);
			this->Controls->Add(this->checkBox5);
			this->Controls->Add(this->checkBox6);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox4);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Busmaster";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

				 //if ()


				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) 
					 {

						 accelerate_vehicle();

						 send_EngineControl01_message();
						 //progressBar1->Step = 10;
						 //progressBar1->PerformStep();
						 progressBar1->Value = signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
						 std::string VehicleSpeed;
						 std::stringstream out;
						 out << signal_VehicleSpeed;
						 VehicleSpeed = out.str();
						 std::string Motorspeed;
						 std::stringstream out2;
						 out2 << signal_MotorSpeed;
						 Motorspeed = out2.str();



						 std::string s1 = VehicleSpeed;
						 std::string s2 = Motorspeed;

						 label2->Text = marshal_as<String^>(s1);
						 label5->Text = marshal_as<String^>(s2);


						 //label2->Text = VehicleSpeed;
					 }
			 }	




	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();

				 objBUSMASTERApp->Connect(true);
				 // objBUSMASTERApp->LoadAllDll();

				 button4->Visible = false;
				 button3->Visible = true;
				 std::string s1 = "0";
				 label2->Text = marshal_as<String^>(s1);
				 label5->Text = marshal_as<String^>(s1);

				 s1 = "Start";
				 label8->Text = marshal_as<String^>(s1);



			 }
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 //objBUSMASTERApp->UnLoadAllDll();
				 objBUSMASTERApp->Connect(false);

				 button4->Visible = true;
				 button3->Visible = false;
				 std::string s1 = "0";
				 label2->Text = marshal_as<String^>(s1);
				 label5->Text = marshal_as<String^>(s1);

				 s1 = "Off";
				 label8->Text = marshal_as<String^>(s1);




			 }

	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 set_vehicle_off();
						 send_BodyComputer01_message();
					 }
					 std::string s1 = "0";
					 label2->Text = marshal_as<String^>(s1);
					 label5->Text = marshal_as<String^>(s1);

					 s1 = "off";
					 label8->Text = marshal_as<String^>(s1);

			 }
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 set_vehicle_ignition_on();
						 send_BodyComputer01_message();
						 progressBar1->Value =signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
					 }
					 std::string s1 = "0";
					 label2->Text = marshal_as<String^>(s1);
					 label5->Text = marshal_as<String^>(s1);

					 s1 = "On";
					 label8->Text = marshal_as<String^>(s1);
			 }
	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 set_vehicle_engine_running();
						 send_BodyComputer01_message();
						 progressBar1->Value =signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
						 std::string s1 = "0";
						 label2->Text = marshal_as<String^>(s1);
						 label5->Text = marshal_as<String^>(s1);

						 s1 = "P";
						 label8->Text = marshal_as<String^>(s1);
					 }
			 }
	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {CAN_MonitorApp::Application^ objBUSMASTERApp; 
			 CAN_MonitorApp::Application^ objBUSMASTERApp1;	
			 objBUSMASTERApp1 = gcnew CAN_MonitorApp::Application();
			 HRESULT hr;
			 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
				 IID_IApplication, (void**)&m_IApplication);
			 if(SUCCEEDED(hr))
				 if (m_IApplication) {
					 set_vehicle_switch_into_neutral_gear();
					 send_BodyComputer01_message();
					 progressBar1->Value =signal_VehicleSpeed;
					 progressBar3->Value = signal_MotorSpeed;
					 std::string s1 = "0";
					 label2->Text = marshal_as<String^>(s1);
					 label5->Text = marshal_as<String^>(s1);

					 s1 = "N";
					 label8->Text = marshal_as<String^>(s1);
				 }
			 }
	private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp;
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 set_vehicle_switch_into_drive_gear();
						 send_BodyComputer01_message();
						 progressBar1->Value =signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
						 std::string s1 = "0";
						 label2->Text = marshal_as<String^>(s1);
						 label5->Text = marshal_as<String^>(s1);

						 s1 = "D";
						 label8->Text = marshal_as<String^>(s1);
					 }

			 }
	private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 set_vehicle_switch_into_reverse_gear();
						 send_BodyComputer01_message();
						 progressBar1->Value =signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
						 std::string s1 = "0";
						 label2->Text = marshal_as<String^>(s1);
						 label5->Text = marshal_as<String^>(s1);

						 s1 = "R";
						 label8->Text = marshal_as<String^>(s1);
					 }
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {
						 decelerate_vehicle();
						 send_BodyComputer01_message();

						 progressBar1->Value =signal_VehicleSpeed;
						 progressBar3->Value = signal_MotorSpeed;
						 std::string VehicleSpeed;
						 std::string MotorSpeed;
						 std::stringstream out;
						 std::stringstream out2;
						 out << signal_VehicleSpeed;
						 VehicleSpeed = out.str();
						 out2 << signal_MotorSpeed;
						 MotorSpeed = out2.str();



						 std::string s1 = VehicleSpeed;
						 std::string s2 = MotorSpeed;
						 label2->Text = marshal_as<String^>(s1);
						 label5->Text = marshal_as<String^>(s2);
					 }
			 }
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_lamp_left();
						 send_BodyComputer01_message();
					 }
			 }
	private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_lamp_right();
						 send_BodyComputer01_message();
					 }
			 }
	private: System::Void checkBox6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_day_light();
						 send_BodyComputer01_message();
					 }
			 }
	private: System::Void checkBox5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_high_beam_light();
						 send_BodyComputer01_message();
					 }
			 }
	private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_esp();
						 send_BreakingSystem01_message();
					 }

			 }

	private: System::Void checkBox7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_brake();
						 send_BreakingSystem01_message();
					 }
			 }
	private: System::Void checkBox3_CheckedChanged_1(System::Object^  sender, System::EventArgs^  e) {
				 CAN_MonitorApp::Application^ objBUSMASTERApp; 
				 objBUSMASTERApp = gcnew CAN_MonitorApp::Application();
				 HRESULT hr;
				 hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_LOCAL_SERVER,
					 IID_IApplication, (void**)&m_IApplication);
				 if(SUCCEEDED(hr))
					 if (m_IApplication) {

						 set_abs();
						 send_BreakingSystem01_message();
					 }
			 }
	};
}

