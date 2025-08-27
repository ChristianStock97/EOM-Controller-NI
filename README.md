# EOM_Controller_NI

C++ source for the EOM controller DLL (NI/DAQ-based).  
Built with Visual Studio (MSVC). Outputs a DLL used by the Python `EOM-GUI` project.

## Build
Open `EOM_Controller_NI.sln` in Visual Studio and build the DLL (x64/Release).

## Exports
- `EOM_Create`
- `EOM_Start`
- `EOM_Stop`
- `EOM_GetValue` (double* diode, double* bias, bool* laser_on)
- `EOM_Destroy`
