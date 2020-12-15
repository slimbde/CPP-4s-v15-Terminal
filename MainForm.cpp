#include "MainForm.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace Terminal;

[STAThreadAttribute]
int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm());



	return 0;
}