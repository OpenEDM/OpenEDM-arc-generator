// Automatically generated C++ file on Sat Sep 27 22:44:21 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD gap_controller.cpp kernel32.lib

#include <cmath>
#include <malloc.h>

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)          = 0; // print message like printf() but exit(0) afterward

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

// FSM state description.
enum STATE {
   IDLE,       // No conduction.
   IGNITION,   // Voltage above breakdown, waiting for the ignition delay to end.
   ARC         // Arc conduction.
};

// Persistent state for the controller. An instance of this struct is allocated once per
// opaque pointer and retained between calls so the controller can maintain state across time steps.
struct sCONTROLLER
{
   STATE state;                     // Current FSM state.
   double ignition_start_timestamp; // Time when ignition phase began.
   double ignition_delay;           // Randomly sampled ignition delay for this event.
};

// Uniform random double in [min, max].
double random_range(double min, double max) {
   // Generate a random integer from 0 to RAND_MAX
   int random_int = rand();
   // Normalize it to the range [0.0, 1.0]
   double random_zero_to_one = static_cast<double>(random_int) / RAND_MAX;
   // Scale it to the desired range [min, max]
   return min + random_zero_to_one * (max - min);
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef TEST
#undef I_arc
#undef V_arc
#undef ARC_CTRL

extern "C" __declspec(dllexport) void gap_controller(struct sCONTROLLER **opaque, double t, union uData *data)
{
   double  I_ARC             = data[ 0].d; // input
   double  V_ARC             = data[ 1].d; // input
   double  V_gap_br          = data[ 2].d; // input parameter
   double  t_ign_delay_min   = data[ 3].d; // input parameter
   double  I_arc_min         = data[ 4].d; // input parameter
   double  t_ign_delay_max   = data[ 5].d; // input parameter
   double  t_startup_delay   = data[ 6].d; // input parameter
   double  t_short_timestamp = data[ 7].d; // input parameter
   double &TEST              = data[ 8].d; // output
   bool   &ARC_CTRL          = data[ 9].b; // output
   bool   &SHORT_CTRL        = data[10].b; // output

   // Validate parameters early.
   if (t_ign_delay_max < t_ign_delay_min) {
      EXIT("Igntion delays are incorrect, max < min");
   }

   if (t < t_startup_delay) {
      ARC_CTRL = false;
      return;
   }

   if (t > t_short_timestamp) {
      ARC_CTRL = false;
      SHORT_CTRL = true;
      return;
   }

   // Allocate persistent state if this is the first call for this opaque pointer.
   // QSPICE passes a pointer-to-pointer so we can store persistent state across calls.
   if(!*opaque) {
      *opaque = (struct sCONTROLLER *) calloc(sizeof(struct sCONTROLLER), 1);
      struct sCONTROLLER *instance = *opaque;
      instance->state = IDLE; // Start in IDLE.
      instance->ignition_start_timestamp = 0.0; // Clear timestamp.
   }

   struct sCONTROLLER *instance = *opaque;

   // Simple FSM implementing gap behaviour.
   switch(instance->state) {
      case IDLE:
         // Enter ignition when voltage exceeds breakdown voltage.
         if (V_ARC > V_gap_br) {
            instance->state = IGNITION;
            instance->ignition_start_timestamp = t;
            // Sample a random ignition delay between min and max.
            instance->ignition_delay = random_range(t_ign_delay_min, t_ign_delay_max);
         }
         break;
      case IGNITION:
         // Abort ignition if voltage falls below breakdown voltage.
         if (V_ARC < V_gap_br) {
            instance->state = IDLE;
         // If delay elapsed while V_arc stays above breakdown voltage, arc starts.
         } else if (t - instance->ignition_start_timestamp > instance->ignition_delay) {
            instance->state = ARC;
         }
         break;
      case ARC:
         // Abort arc when arc current falls below min arc current.
         if (I_ARC < I_arc_min) {
            instance->state = IDLE;
         }
         break;
   }

   TEST = instance->state;
   // ARC_CTRL is true only when the instance->state is ARC.
   ARC_CTRL = (instance->state == ARC);
}
