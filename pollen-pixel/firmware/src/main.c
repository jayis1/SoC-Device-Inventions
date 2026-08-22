#include <stdio.h>
#include "pipeline.h"
#include "device.h"

static void print_report(const pp_report_t *report)
{
    printf("Pollen Pixel demo report\n");
    printf("particles: %zu\n", report->particle_count);
    printf("concentration: %.1f grains/m^3\n", report->grains_per_m3);
    printf("dominant size bin center: %.1f um\n", report->dominant_um);
    printf("allergen index: %.1f/100\n", report->allergen_index);

    for (size_t i = 0; i < report->particle_count; ++i) {
        const pp_particle_t *p = &report->particles[i];
        printf("[%02zu] %-13s area=%u circ=%.2f elong=%.2f uv=%.2f edge=%.2f bbox=(%u,%u)-(%u,%u)\n",
               i,
               pp_class_name(p->klass),
               p->area_px,
               p->circularity,
               p->elongation,
               p->uv_ratio,
               p->edge_density,
               p->min_x,
               p->min_y,
               p->max_x,
               p->max_y);
    }
}

int main(void)
{
    pp_capture_t capture;
    pp_report_t report;

    pp_make_demo_capture(&capture);
    pp_pipeline_run(&capture, &report);
    print_report(&report);
    return 0;
}
