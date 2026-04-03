// Automatically generated C++ file on Thu Sep 25 18:44:05 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD controller.cpp kernel32.lib

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
   STATE_1  = 1,  // INDUCTOR CHARGING
   STATE_2,       // IONIZATION CAP DISCHARGE
   STATE_3,       // IONIZATION CAP CHARGE
   STATE_4,       // HIGH EFFICIENCY IONIZATION
   STATE_5,       // ARC TRANSIENT
   STATE_6,       // ARC (POWER FROM INDUCTOR & PS)
   STATE_7,       // ARC (POWER FROM INDUCTOR)
   STATE_8,       // PAUSE
   STATE_COUNT    // Number of states.
};

typedef struct {
   bool Q1;
   bool Q2;
   bool Q3;
   bool Q4;
} QCTRL;

static const QCTRL mapping[STATE_COUNT] = {
   [STATE_1] = {.Q1 = true,    .Q2 = false,   .Q3 = false,   .Q4 = true  }, // INDUCTOR CHARGING
   [STATE_2] = {.Q1 = true,    .Q2 = false,   .Q3 = true,    .Q4 = true  }, // IONIZATION CAP DISCHARGE
   [STATE_3] = {.Q1 = true,    .Q2 = false,   .Q3 = false,   .Q4 = false }, // IONIZATION CAP CHARGE
   [STATE_4] = {.Q1 = true,    .Q2 = false,   .Q3 = true,    .Q4 = false }, // HIGH EFFICIENCY IONIZATION
   [STATE_5] = {.Q1 = true,    .Q2 = false,   .Q3 = true,    .Q4 = false }, // ARC TRANSIENT
   [STATE_6] = {.Q1 = true,    .Q2 = false,   .Q3 = false,   .Q4 = false }, // ARC (POWER FROM INDUCTOR & PS)
   [STATE_7] = {.Q1 = false,   .Q2 = true,    .Q3 = false,   .Q4 = false }, // ARC (POWER FROM INDUCTOR)
   [STATE_8] = {.Q1 = false,   .Q2 = true,    .Q3 = false,   .Q4 = true  }, // PAUSE
};

// Persistent state for the controller. An instance of this struct is allocated once per
// opaque pointer and retained between calls so the controller can maintain state across time steps.
struct sCONTROLLER
{
   STATE state; // Current FSM state.
   int i; // Current period index. The duration of each period is T.
   double last_transition_timestamp; // Time when last state transition happened.
   double ionization_started_timestamp;
   double V_cap_pause;
   double I_ind_pause;
   double dt_cap;
   double dt_ind;
};

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef H_CTRL
#undef L_CTRL
#undef I

void go_to_next_state(struct sCONTROLLER *instance, double t_rel, STATE next_state) {
   instance->last_transition_timestamp = t_rel;
   instance->state = next_state;
}

extern "C" __declspec(dllexport) void generator_controller(struct sCONTROLLER **opaque, double t, union uData *data)
{
   double  V_CAP          = data[ 0].d; // input
   double  V_ARC          = data[ 1].d; // input
   double  CS_COIL        = data[ 2].d; // input
   double  CS_ARC         = data[ 3].d; // input
   double  T              = data[ 4].d; // input parameter
   double  I_ind_sp       = data[ 5].d; // input parameter
   double  t_on           = data[ 6].d; // input parameter
   double  t_off          = data[ 7].d; // input parameter
   double  t_ind_chrg_max = data[ 8].d; // input parameter
   double  V_cap_sp       = data[ 9].d; // input parameter
   double  t_cap_chrg_max = data[10].d; // input parameter
   double  I_ind_sp_hyst  = data[11].d; // input parameter
   double  V_cap_sp_hyst  = data[12].d; // input parameter
   double  t_ind_chrg_min = data[13].d; // input parameter
   double  L              = data[14].d; // input parameter
   double  V_main         = data[15].d; // input parameter
   double  C              = data[16].d; // input parameter
   bool   &Q1_CTRL        = data[17].b; // output
   bool   &Q2_CTRL        = data[18].b; // output
   double &TEST           = data[19].d; // output
   bool   &Q3_CTRL        = data[20].b; // output
   bool   &Q4_CTRL        = data[21].b; // output

   // Allocate persistent state if this is the first call for this opaque pointer.
   // QSPICE passes a pointer-to-pointer so we can store persistent state across calls.
   if(!*opaque) {
      *opaque = (struct sCONTROLLER *) calloc(sizeof(struct sCONTROLLER), 1);

      struct sCONTROLLER *instance = *opaque;
      instance->state = STATE_1; // Start in STATE_1.
   }

   struct sCONTROLLER *instance = *opaque;

   // Calculate the index of the period we are in.
   int i = floor(t / T);

   // On the first sample of a new period, re-enable charging and record the new period index.
   if (i != instance->i) {
      instance->i = i;
      // instance->charging = true;
      instance->state = STATE_1;

      double V_cap_error = V_cap_sp - instance->V_cap_pause;
      instance->dt_cap = C * fabs(V_cap_error) / instance->I_ind_pause;

      double I_ind_error = I_ind_sp - instance->I_ind_pause;
      instance->dt_ind = L * I_ind_error / V_main;

      if (instance->dt_ind < 0) {
         instance->dt_ind = 0;
      }
   }

   // Compute time relative to the start of the current period (0 <= t_rel < T).
   double t_rel = t - i * T;

   double state_duration = t_rel - instance->last_transition_timestamp;
   double actual_t_on = t_rel - instance->ionization_started_timestamp;

   switch (instance->state) {
      case STATE_1: // INDUCTOR CHARGING
         if (t_rel < t_ind_chrg_min) {
            break;
         }
         if (t_rel > instance->dt_ind || t_rel > t_ind_chrg_max) {
            if (instance->V_cap_pause > V_cap_sp) {
               go_to_next_state(instance, t_rel, STATE_2);
            } else {
               instance->ionization_started_timestamp = t_rel;
               go_to_next_state(instance, t_rel, STATE_3);
            }
         }
         break;
      case STATE_2: // IONIZATION CAP DISCHARGE
         if (state_duration > instance->dt_cap || state_duration > t_cap_chrg_max) {
            instance->ionization_started_timestamp = t_rel;
            go_to_next_state(instance, t_rel, STATE_4);
         }
         break;
      case STATE_3: // IONIZATION CAP CHARGE
         if (CS_ARC > I_ind_sp * 0.2) { // We consider the arc started if the actual arc current is more than 20% of the inductor current setpoint.
            go_to_next_state(instance, t_rel, STATE_6);
         } else if (state_duration > instance->dt_cap || state_duration > t_cap_chrg_max) {
            go_to_next_state(instance, t_rel, STATE_4);
         }
         break;
      case STATE_4: // HIGH EFFICIENCY IONIZATION
         if (actual_t_on > t_on) {
            go_to_next_state(instance, t_rel, STATE_8);
         } else if (CS_ARC > I_ind_sp * 0.2) { // We consider the arc started if the actual arc current is more than 20% of the inductor current setpoint.
            go_to_next_state(instance, t_rel, STATE_6);
         }
         break;

      // There is nothing to do in STATE_5, as we must immediately transition to STATE_6.
      // case STATE_5: // ARC TRANSIENT

      case STATE_6: // ARC (POWER FROM INDUCTOR & PS)
         if (actual_t_on > t_on) {
            go_to_next_state(instance, t_rel, STATE_8);
         } else if (CS_ARC > I_ind_sp + I_ind_sp_hyst / 2.0) {
            go_to_next_state(instance, t_rel, STATE_7);
         }
         break;
      case STATE_7: // ARC (POWER FROM INDUCTOR)
         if (actual_t_on > t_on) {
            go_to_next_state(instance, t_rel, STATE_8);
         } else if (CS_ARC < I_ind_sp - I_ind_sp_hyst / 2.0) {
            go_to_next_state(instance, t_rel, STATE_6);
         }
         break;
      case STATE_8: // PAUSE
         instance->V_cap_pause = V_CAP;
         instance->I_ind_pause = CS_COIL;
         break;
      default:
         EXIT("Unexpected FSM state");
         break;
   }

   if (instance->state < 0 || instance->state >= STATE_COUNT) {
      EXIT("Wrong FSM state");
   }

   const QCTRL *m = &mapping[instance->state];
   Q1_CTRL = m->Q1;
   Q2_CTRL = m->Q2;
   Q3_CTRL = m->Q3;
   Q4_CTRL = m->Q4;

   // Store relative time into TEST output (for debugging/monitoring).
   TEST = instance->state;
}
