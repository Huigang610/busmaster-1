/**
 * Main project file.
 */

/* Project includes */
#include "stdafx.h"
#include "Signal_Interaction.h"			/* contains send/receive behavior */
#include "Panel.h"						/* contains GUI */

using namespace Test;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	/* Initialize message structures */
	initialize_messages();

	/* Enabling Windows XP visual effects before any controls are created */
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false); 

	/* Create the main window and run it */
	System::Windows::Forms::Application::Run(gcnew Form1());
	return 0;
}
