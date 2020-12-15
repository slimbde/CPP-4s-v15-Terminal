#pragma once
#include "Models\Terminals.h"

namespace Terminal
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;



	public ref class MainForm : public System::Windows::Forms::Form
	{
		IPublisher^ terminal;

	public:
		MainForm(void)
		{
			InitializeComponent();

			auto pFactory = gcnew PassengerFactory();
			auto gFactory = gcnew GatesFactory();

			terminal = gcnew GatesTerminal(pFactory, gFactory);
			timer1->Interval = 200;
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

	private: System::Windows::Forms::Button^  buttonAppend;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  buttonStart;
	private: System::Windows::Forms::GroupBox^  groupCount;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ListBox^  listBox1;

	private: void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
		this->listBox1 = (gcnew System::Windows::Forms::ListBox());
		this->buttonStart = (gcnew System::Windows::Forms::Button());
		this->groupCount = (gcnew System::Windows::Forms::GroupBox());
		this->buttonAppend = (gcnew System::Windows::Forms::Button());
		this->groupCount->SuspendLayout();
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
		this->listBox1->Size = System::Drawing::Size(244, 170);
		this->listBox1->TabIndex = 0;
		// 
		// buttonStart
		// 
		this->buttonStart->Enabled = false;
		this->buttonStart->Location = System::Drawing::Point(138, 12);
		this->buttonStart->Name = L"buttonStart";
		this->buttonStart->Size = System::Drawing::Size(131, 23);
		this->buttonStart->TabIndex = 1;
		this->buttonStart->Text = L"Начать регистрацию";
		this->buttonStart->UseVisualStyleBackColor = true;
		this->buttonStart->Click += gcnew System::EventHandler(this, &MainForm::buttonStart_Click);
		// 
		// groupCount
		// 
		this->groupCount->Controls->Add(this->listBox1);
		this->groupCount->Location = System::Drawing::Point(12, 41);
		this->groupCount->Name = L"groupCount";
		this->groupCount->Padding = System::Windows::Forms::Padding(10, 3, 3, 3);
		this->groupCount->Size = System::Drawing::Size(257, 189);
		this->groupCount->TabIndex = 3;
		this->groupCount->TabStop = false;
		this->groupCount->Text = L"Людей в очереди:";
		// 
		// buttonAppend
		// 
		this->buttonAppend->Location = System::Drawing::Point(12, 12);
		this->buttonAppend->Name = L"buttonAppend";
		this->buttonAppend->Size = System::Drawing::Size(109, 23);
		this->buttonAppend->TabIndex = 4;
		this->buttonAppend->Text = L"Добавить людей";
		this->buttonAppend->UseVisualStyleBackColor = true;
		this->buttonAppend->Click += gcnew System::EventHandler(this, &MainForm::buttonAppend_Click);
		// 
		// MainForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(281, 242);
		this->Controls->Add(this->buttonAppend);
		this->Controls->Add(this->groupCount);
		this->Controls->Add(this->buttonStart);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
		this->MaximizeBox = false;
		this->Name = L"MainForm";
		this->Text = L"Терминал";
		this->groupCount->ResumeLayout(false);
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
			groupCount->Text = info->UnhandledInfo;
			listBox1->Items->AddRange(info->GateInfos->ToArray());
			listBox1->Items->Add("");
			listBox1->Items->Add(String::Concat("Время регистрации: ", time));
		}
		catch (Exception^ ex)
		{
			timer1->Enabled = false;

			buttonAppend->Enabled = true;
			buttonStart->Enabled = false;

			auto summary = terminal->GetSummary();
			listBox1->Items->Add("");

			for each (KeyValuePair<int, double> pair in summary->AvgHandlingTime)
				listBox1->Items->Add("Окно " + pair.Key + ": Cр. время обслуживания = " + Math::Round(pair.Value, 2));

			listBox1->Items->Add("");
			MessageBox::Show(ex->Message);
		}
	}
	private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Enabled = true;
		buttonAppend->Enabled = false;
		buttonStart->Enabled = false;
	}
	private: System::Void buttonAppend_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int count = 0;
		int::TryParse(groupCount->Text->Split(':')[1]->Trim(), count);
		groupCount->Text = "Людей в очереди: " + (count + terminal->Increase());

		buttonStart->Enabled = true;
	}
	};
}
