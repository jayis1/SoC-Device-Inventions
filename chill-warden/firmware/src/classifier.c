#include <stdio.h>
#include <string.h>

#include "classifier.h"

const char *cw_mode_string(cw_mode_t mode)
{
    switch (mode) {
    case CW_MODE_COOL: return "cool";
    case CW_MODE_HEAT: return "heat";
    case CW_MODE_FREEZER: return "freezer";
    case CW_MODE_IDLE: return "idle";
    default: return "unknown";
    }
}

const char *cw_fault_string(cw_fault_t fault)
{
    switch (fault) {
    case CW_FAULT_NORMAL: return "NORMAL";
    case CW_FAULT_AIRFLOW_RESTRICTED: return "AIRFLOW_RESTRICTED";
    case CW_FAULT_LOW_CHARGE_SUSPECT: return "LOW_CHARGE_SUSPECT";
    case CW_FAULT_BEARING_WEAR: return "BEARING_WEAR";
    case CW_FAULT_LIQUID_SLUGGING: return "LIQUID_SLUGGING";
    case CW_FAULT_SHORT_CYCLING: return "SHORT_CYCLING";
    case CW_FAULT_DEFROST_ISSUE: return "DEFROST_ISSUE";
    default: return "UNKNOWN";
    }
}

static void set_result(cw_result_t *result, cw_fault_t fault, double risk,
                       const char *reason, const char *action)
{
    result->fault = fault;
    result->risk_score = risk;
    (void)snprintf(result->reason, sizeof(result->reason), "%s", reason);
    (void)snprintf(result->action, sizeof(result->action), "%s", action);
}

void cw_classify_frame(const cw_frame_t *frame, cw_result_t *result)
{
    if (frame->cycle_period_s < 300.0) {
        set_result(result,
                   CW_FAULT_SHORT_CYCLING,
                   81.0,
                   "Starts are occurring too frequently for stable refrigeration operation.",
                   "Check thermostat placement, anti-short-cycle delay, and low-pressure safety logic.");
        return;
    }

    if (frame->superheat_proxy_c < 2.0 && frame->piezo_db > 70.0) {
        set_result(result,
                   CW_FAULT_LIQUID_SLUGGING,
                   90.0,
                   "Shell-borne impact energy is high while superheat proxy is near zero.",
                   "Inspect TXV bulb, evaporator loading, and floodback / crankcase-heater conditions.");
        return;
    }

    if (frame->vibration_g > 1.6 && frame->acoustic_db > 72.0) {
        set_result(result,
                   CW_FAULT_BEARING_WEAR,
                   82.0,
                   "Vibration and broadband acoustics indicate rough rotating machinery.",
                   "Inspect condenser fan and compressor bearings; compare with a known-good unit.");
        return;
    }

    if (frame->superheat_proxy_c > 18.0 && frame->current_a < 5.0 && frame->condenser_split_c < 28.0) {
        set_result(result,
                   CW_FAULT_LOW_CHARGE_SUSPECT,
                   76.0,
                   "High superheat proxy coincides with low current and weak condenser split.",
                   "Check refrigerant charge, inspect coils for oil staining, and verify metering device behavior.");
        return;
    }

    if (frame->condenser_split_c > 45.0 && frame->discharge_c > 85.0 && frame->acoustic_db > 68.0) {
        set_result(result,
                   CW_FAULT_AIRFLOW_RESTRICTED,
                   79.0,
                   "Discharge temperature and condenser split are elevated, consistent with poor heat rejection.",
                   "Clean condenser coil, confirm fan rotation/speed, and check for discharge-air recirculation.");
        return;
    }

    if (frame->mode == CW_MODE_FREEZER && frame->ambient_c > 20.0 && frame->rh_pct > 80.0 && frame->suction_c < -10.0 && frame->cycle_period_s > 3600.0) {
        set_result(result,
                   CW_FAULT_DEFROST_ISSUE,
                   73.0,
                   "Long freezer run periods under humid conditions suggest defrost underperformance.",
                   "Inspect defrost heater, termination sensor, timer/board output, and evaporator ice pattern.");
        return;
    }

    set_result(result,
               CW_FAULT_NORMAL,
               14.0,
               "Current, temperatures, and mechanical signatures remain within the expected envelope.",
               "Continue monitoring or archive this log as a healthy baseline.");
}
