#include <stddef.h>

#include "classifier.h"
#include "pulse_model.h"

const char *pp_fault_name(pp_fault_t fault)
{
    switch (fault) {
    case PP_HEALTHY:
        return "FENCE_HEALTHY";
    case PP_VEGETATION_LOADING:
        return "VEGETATION_LOADING";
    case PP_WET_INSULATOR_LEAKAGE:
        return "WET_INSULATOR_LEAKAGE";
    case PP_OPEN_GATE_OR_BREAK:
        return "OPEN_GATE_OR_BREAK";
    case PP_WEAK_ENERGIZER_BATTERY:
        return "WEAK_ENERGIZER_BATTERY";
    case PP_SURGE_DISTURBANCE:
        return "SURGE_DISTURBANCE";
    case PP_FENCE_DOWN:
        return "FENCE_DOWN";
    default:
        return "UNKNOWN";
    }
}

static void clear_notes(pp_result_t *result)
{
    for (size_t i = 0; i < PP_MAX_NOTES; ++i) {
        result->notes[i] = NULL;
    }
}

void pp_classify_frame(const pp_frame_t *frame, pp_result_t *result)
{
    pp_extract_features(frame, &result->features);
    clear_notes(result);

    if (frame->pulse_missing || frame->pulse_peak_kv < 0.8) {
        result->fault = PP_FENCE_DOWN;
        result->confidence = 0.99;
        result->summary = "No usable pulse detected; inspect energizer output and downstream continuity.";
        result->notes[0] = "Walk line for broken conductor or fallen netting.";
        result->notes[1] = "Confirm local ground system is intact.";
        return;
    }

    if (frame->surge_flag || frame->pulse_interval_s < 0.6 || frame->rise_time_us < 6.0) {
        result->fault = PP_SURGE_DISTURBANCE;
        result->confidence = 0.93;
        result->summary = "Non-periodic fast transient detected; likely lightning or external surge disturbance.";
        result->notes[0] = "Check surge diverter and fence energizer arrestor.";
        result->notes[1] = "Ignore single event unless repeated.";
        return;
    }

    if (frame->gate_open || (frame->pulse_peak_kv < 2.0 && frame->current_peak_a < 0.5)) {
        result->fault = PP_OPEN_GATE_OR_BREAK;
        result->confidence = frame->gate_open ? 0.98 : 0.84;
        result->summary = "Pulse collapsed with little delivered current; an open gate or conductor break is likely.";
        result->notes[0] = "Inspect gate handles, jumpers, and corner splices first.";
        result->notes[1] = "If gate is closed, check for cracked underground cable sections.";
        return;
    }

    if (frame->battery_v < 3.05 && frame->pulse_peak_kv < 4.2 && result->features.leakage_ratio < 0.55) {
        result->fault = PP_WEAK_ENERGIZER_BATTERY;
        result->confidence = 0.88;
        result->summary = "Pulse strength is falling without a matching leakage rise; energizer supply weakness is suspected.";
        result->notes[0] = "Check solar charger, battery resting voltage, and panel shading.";
        result->notes[1] = "Compare with a near-energizer node if available.";
        return;
    }

    if (result->features.wetness_factor > 0.72 && frame->pulse_peak_kv < 5.4 && result->features.leakage_ratio < 0.75) {
        result->fault = PP_WET_INSULATOR_LEAKAGE;
        result->confidence = 0.81;
        result->summary = "Humidity-correlated leakage is present; suspect wet insulators or contamination tracks.";
        result->notes[0] = "Inspect porcelain/plastic stand-offs for film, algae, or cracked surfaces.";
        result->notes[1] = "Re-check after conditions dry to confirm.";
        return;
    }

    if (result->features.leakage_ratio > 0.80 && frame->pulse_peak_kv < 6.0 && frame->decay_time_us > 200.0) {
        result->fault = PP_VEGETATION_LOADING;
        result->confidence = 0.91;
        result->summary = "Current draw is high relative to pulse voltage; conductive grass or brush contact is likely.";
        result->notes[0] = "Trim undergrowth near low spots, corners, and wet creek crossings.";
        result->notes[1] = "Look for netting collapsed into dense vegetation.";
        return;
    }

    result->fault = PP_HEALTHY;
    result->confidence = 0.95;
    result->summary = "Pulse amplitude, interval, and leakage trend are within expected limits.";
    result->notes[0] = "No immediate action required.";
    result->notes[1] = "Continue periodic trend monitoring for seasonal loading changes.";
}
