#include "mainform.h" // Ensure this matches your main form's header file
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
 void main() {
    // Enable visual styles for the application
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Launch the main form
    DSP::mainform mainForm; // Adjust namespace if necessary
    Application::Run(% mainForm);
}
