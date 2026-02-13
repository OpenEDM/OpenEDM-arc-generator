// Automatically generated C++ file on Thu Sep 25 18:44:05 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD controller.cpp kernel32.lib

#include <cmath>
#include <malloc.h>

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// Persistent state for the controller. An instance of this struct is allocated once per
// opaque pointer and retained between calls so the controller can maintain state across time steps.
struct sCONTROLLER
{
   int i; // Current period index. The duration of each period is T.
   bool charging; // Inductor's charging state. If true, charging is currently happening.
};

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef H_CTRL
#undef L_CTRL
#undef I

extern "C" __declspec(dllexport) void generator_controller(struct sCONTROLLER **opaque, double t, union uData *data)
{
   double  I_act      = data[0].d; // input
   double  T          = data[1].d; // input parameter
   double  I_sp       = data[2].d; // input parameter
   double  t_on       = data[3].d; // input parameter
   double  t_off      = data[4].d; // input parameter
   double  t_chrg_max = data[5].d; // input parameter
   bool   &H_CTRL     = data[6].b; // output
   bool   &L_CTRL     = data[7].b; // output
   double &TEST       = data[8].d; // output

   // Allocate persistent state if this is the first call for this opaque pointer.
   // QSPICE passes a pointer-to-pointer so we can store persistent state across calls.
   if(!*opaque)
      *opaque = (struct sCONTROLLER *) calloc(sizeof(struct sCONTROLLER), 1);

   struct sCONTROLLER *instance = *opaque;

   // Calculate the index of the period we are in.
   int i = floor(t / T);

   // On the first sample of a new period, re-enable charging and record the new period index.
   if (i != instance->i) {
      instance->i = i;
      instance->charging = true;
   }

   // Compute time relative to the start of the current period (0 <= t_rel < T).
   double t_rel = t - i * T;

   // Disable charging (H_CTRL = false) for the current period if either:
   // 1. The relative time exceeds the configured maximum charging interval within the period
   // OR
   // 2. The measured current exceeds the current setpoint.
   if (t_rel > t_chrg_max || I_act > I_sp) {
       instance->charging = false;
   }

   // Store relative time into TEST output (for debugging/monitoring).
   TEST = t_rel;

   // H_CTRL follows the persistent `charging` flag.
   H_CTRL = instance->charging;

   // L_CTRL is true outside the centered `t_on` window around T/2.
   // I.e., L_CTRL is true when t_rel is inside any ot these two intervals:
   // 1. (0, T/2 - t_on/2).
   // 2. (T/2 + t_on/2, T).
   L_CTRL = (t_rel < (T / 2 - t_on / 2)) || (t_rel > (T / 2 + t_on / 2));
}
