// EOM_Regulator_API.cpp
// Exposes a plain C API for EOM_Regulator so it can be used from other languages.
// Build this into your DLL together with the object file that defines EOM_Regulator.

#include "pch.h"
#include "DAC_module.h"     // Must declare EOM_Regulator here or in an included header

// If you prefer __stdcall for interop (e.g., with C#/PInvoke), uncomment:
// #include <windows.h>
#define DLL_CALL __stdcall
// Otherwise, default to cdecl:
#define DLL_CALL

#ifdef __cplusplus
extern "C" {
#endif

    // Opaque handle type returned to callers
    typedef void* EOM_HANDLE;

    // Create/initialize an instance of EOM_Regulator.
    // Returns nullptr on failure.
    __declspec(dllexport) EOM_HANDLE DLL_CALL EOM_Create(
        short board_idx,
        double dac_min, double dac_max,
        double adc_min, double adc_max,
        double min_threshold,
        double max_threshold)
    {
        try {
            // Construct and return as opaque pointer
            auto* obj = new EOM_Regulator(
                board_idx,
                dac_min, dac_max,
                adc_min, adc_max,
                min_threshold,
                max_threshold
            );
            return static_cast<EOM_HANDLE>(obj);
        }
        catch (...) {
            return nullptr;
        }
    }

    // Start regulation loop (spawns the worker thread inside the object).
    // Returns true on success, false on failure or if handle is null.
    __declspec(dllexport) bool DLL_CALL EOM_Start(EOM_HANDLE handle)
    {
        if (!handle) return false;
        auto* obj = static_cast<EOM_Regulator*>(handle);
        try {
            return obj->start();
        }
        catch (...) {
            return false;
        }
    }

    // Stop regulation loop and join the worker thread.
    __declspec(dllexport) void DLL_CALL EOM_Stop(EOM_HANDLE handle)
    {
        if (!handle) return;
        auto* obj = static_cast<EOM_Regulator*>(handle);
        try {
            obj->stop();
        }
        catch (...) {
            // swallow exceptions to keep C ABI stable
        }
    }

    // Get the latest diode value and laser state.
    // Outputs are written to *diode_out and *laser_on_out if the pointers are non-null.
    __declspec(dllexport) void DLL_CALL EOM_GetValue(EOM_HANDLE handle, double* diode_out, double* bias_out, bool* laser_on_out)
    {
        if (!handle) return;
        auto* obj = static_cast<EOM_Regulator*>(handle);

        double diode = 0.0;
        double bias = 0.0;
        bool laser = false;

        try {
            obj->get_values(&diode, &bias, &laser);
        }
        catch (...) {
            // On error, leave defaults (0.0 / false)
        }

        if (diode_out) *diode_out = diode;
        if (diode_out) *bias_out = bias;
        if (laser_on_out) *laser_on_out = laser;
    }

    // (Optional) Destroy the instance to free memory.
    // Not explicitly requested, but useful to avoid leaks for long-lived apps.
    __declspec(dllexport) void DLL_CALL EOM_Destroy(EOM_HANDLE handle)
    {
        if (!handle) return;
        auto* obj = static_cast<EOM_Regulator*>(handle);
        try {
            // Ensure the background thread is stopped before delete
            obj->stop();
        }
        catch (...) {
            // ignore
        }
        delete obj;
    }

#ifdef __cplusplus
} // extern "C"
#endif
