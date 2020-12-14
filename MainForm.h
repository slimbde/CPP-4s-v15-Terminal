#pragma once
#include "Models\Terminals.h"

namespace Terminal
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	//using namespace System::Data;
	//using namespace System::Drawing;



	public ref class MainForm : public System::Windows::Forms::Form
	{
		PassengerFactory^ pFactory;
		GatesFactory^ gFactory;
		GatesTerminal^ terminal;

	public:
		MainForm(void)
		{
			InitializeComponent();

			pFactory = gcnew PassengerFactory();
			gFactory = gcnew GatesFactory();
			terminal = gcnew GatesTerminal(pFactory, gFactory);
			timer1->Interval = 500;
		}

#pragma region Windows Form Designer generated code
	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  buttonStart;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ListBox^  listBox1;

	private:

		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// listBox1
			// 
			this->listBox1->BackColor = System::Drawing::SystemColors::Control;
			this->listBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(10, 16);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(172, 137);
			this->listBox1->TabIndex = 0;
			// 
			// buttonStart
			// 
			this->buttonStart->Location = System::Drawing::Point(12, 12);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(131, 23);
			this->buttonStart->TabIndex = 1;
			this->buttonStart->Text = L"Начать регистрацию";
			this->buttonStart->UseVisualStyleBackColor = true;
			this->buttonStart->Click += gcnew System::EventHandler(this, &MainForm::buttonStart_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->listBox1);
			this->groupBox1->Location = System::Drawing::Point(12, 41);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(10, 3, 3, 3);
			this->groupBox1->Size = System::Drawing::Size(185, 156);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Информация";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(208, 209);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->buttonStart);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"Терминал";
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		try
		{
			auto info = terminal->Sync();

			auto ts = TimeSpan::FromSeconds(info->elapsed);
			auto time = ts.Minutes > 0 ? ts.Minutes + " м " + ts.Seconds + " c" : ts.Seconds + " c";

			listBox1->Items->Clear();
			listBox1->Items->Add(info->UnhandledInfo);
			listBox1->Items->Add("");
			listBox1->Items->AddRange(info->GateInfo->ToArray());
			listBox1->Items->Add("");
			listBox1->Items->Add(String::Concat("Время регистрации: ", time));
		}
		catch (Exception^ ex)
		{
			timer1->Enabled = false;
			MessageBox::Show(ex->Message);
		}
	}
	private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Enabled = true;
	}
	};
}
