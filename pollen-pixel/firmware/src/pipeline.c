#include <string.h>
#include <math.h>
#include "pipeline.h"
#include "classifier.h"
#include "flow.h"

static const float k_hist_centers_um[PP_MAX_HIST_BINS] = {8.f, 12.f, 16.f, 20.f, 24.f, 30.f, 40.f, 55.f};

const char *pp_class_name(pp_particle_class_t klass)
{
    switch (klass) {
    case PP_CLASS_GRASS_POLLEN: return "grass";
    case PP_CLASS_TREE_POLLEN: return "tree";
    case PP_CLASS_WEED_POLLEN: return "weed";
    case PP_CLASS_FUNGAL_SPORE: return "fungal_spore";
    case PP_CLASS_DUST: return "dust";
    default: return "unknown";
    }
}

static void stamp_blob(pp_capture_t *capture,
                       int x0,
                       int y0,
                       int x1,
                       int y1,
                       uint8_t bright,
                       uint8_t uv)
{
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            if (x >= 0 && x < PP_FRAME_W && y >= 0 && y < PP_FRAME_H) {
                capture->brightfield[y][x] = bright;
                capture->uv[y][x] = uv;
            }
        }
    }
}

void pp_make_demo_capture(pp_capture_t *capture)
{
    memset(capture, 0, sizeof(*capture));
    capture->flow_lpm = 1.8f;
    capture->sample_seconds = 120.0f;
    capture->sampled_area_mm2 = 0.92f;
    capture->inlet_efficiency = 0.86f;

    stamp_blob(capture, 3, 4, 6, 7, 182, 105);   /* grass pollen */
    stamp_blob(capture, 11, 3, 15, 8, 226, 72);  /* tree pollen */
    stamp_blob(capture, 21, 5, 24, 12, 210, 168);/* fungal spore */
    stamp_blob(capture, 7, 14, 12, 18, 214, 146);/* weed pollen */
    stamp_blob(capture, 18, 15, 20, 17, 150, 38);/* dust */
    stamp_blob(capture, 25, 14, 30, 19, 225, 78);/* tree pollen */

    for (int y = 0; y < PP_FRAME_H; ++y) {
        for (int x = 0; x < PP_FRAME_W; ++x) {
            capture->brightfield[y][x] += (uint8_t)((x * 3 + y * 5) % 9);
            capture->uv[y][x] += (uint8_t)((x * 7 + y * 2) % 5);
        }
    }
}

static float compute_edge_density(const pp_capture_t *capture,
                                  uint8_t min_x,
                                  uint8_t min_y,
                                  uint8_t max_x,
                                  uint8_t max_y)
{
    float edges = 0.0f;
    float count = 0.0f;
    for (uint8_t y = min_y; y <= max_y; ++y) {
        for (uint8_t x = min_x; x <= max_x; ++x) {
            if (x + 1 <= max_x) {
                edges += fabsf((float)capture->brightfield[y][x] - (float)capture->brightfield[y][x + 1]);
                count += 1.0f;
            }
            if (y + 1 <= max_y) {
                edges += fabsf((float)capture->brightfield[y][x] - (float)capture->brightfield[y + 1][x]);
                count += 1.0f;
            }
        }
    }
    return count > 0.0f ? edges / (count * 255.0f) : 0.0f;
}

static void append_particle(const pp_capture_t *capture,
                            pp_report_t *report,
                            uint8_t visited[PP_FRAME_H][PP_FRAME_W],
                            uint8_t seed_x,
                            uint8_t seed_y,
                            uint8_t threshold)
{
    uint8_t queue_x[PP_FRAME_W * PP_FRAME_H];
    uint8_t queue_y[PP_FRAME_W * PP_FRAME_H];
    int head = 0;
    int tail = 0;
    uint32_t sum_bright = 0;
    uint32_t sum_uv = 0;
    uint8_t min_x = seed_x;
    uint8_t max_x = seed_x;
    uint8_t min_y = seed_y;
    uint8_t max_y = seed_y;

    if (report->particle_count >= PP_MAX_PARTICLES) {
        return;
    }

    queue_x[tail] = seed_x;
    queue_y[tail] = seed_y;
    ++tail;
    visited[seed_y][seed_x] = 1;

    pp_particle_t *p = &report->particles[report->particle_count];
    memset(p, 0, sizeof(*p));

    while (head < tail) {
        uint8_t x = queue_x[head];
        uint8_t y = queue_y[head];
        ++head;

        p->area_px++;
        sum_bright += capture->brightfield[y][x];
        sum_uv += capture->uv[y][x];
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;

        static const int8_t dx[4] = {1, -1, 0, 0};
        static const int8_t dy[4] = {0, 0, 1, -1};
        for (int i = 0; i < 4; ++i) {
            int nx = (int)x + dx[i];
            int ny = (int)y + dy[i];
            if (nx < 0 || nx >= PP_FRAME_W || ny < 0 || ny >= PP_FRAME_H) {
                continue;
            }
            if (visited[ny][nx] || capture->brightfield[ny][nx] < threshold) {
                continue;
            }
            visited[ny][nx] = 1;
            queue_x[tail] = (uint8_t)nx;
            queue_y[tail] = (uint8_t)ny;
            ++tail;
        }
    }

    p->min_x = min_x;
    p->max_x = max_x;
    p->min_y = min_y;
    p->max_y = max_y;

    float width = (float)(max_x - min_x + 1);
    float height = (float)(max_y - min_y + 1);
    float perimeter = 2.0f * (width + height);
    p->circularity = (perimeter > 0.0f)
        ? (4.0f * 3.1415926f * (float)p->area_px) / (perimeter * perimeter)
        : 0.0f;
    p->elongation = width > height ? width / height : height / width;
    p->uv_ratio = (sum_bright > 0) ? (float)sum_uv / (float)sum_bright : 0.0f;
    p->edge_density = compute_edge_density(capture, min_x, min_y, max_x, max_y);
    p->klass = pp_classify_particle(p);

    ++report->particle_count;
}

void pp_pipeline_run(const pp_capture_t *capture, pp_report_t *report)
{
    memset(report, 0, sizeof(*report));
    uint8_t visited[PP_FRAME_H][PP_FRAME_W] = {{0}};
    const uint8_t threshold = 120;
    float weighted = 0.0f;

    for (uint8_t y = 0; y < PP_FRAME_H; ++y) {
        for (uint8_t x = 0; x < PP_FRAME_W; ++x) {
            if (!visited[y][x] && capture->brightfield[y][x] >= threshold) {
                append_particle(capture, report, visited, x, y, threshold);
            }
        }
    }

    uint16_t max_bin = 0;
    size_t max_bin_index = 0;
    for (size_t i = 0; i < report->particle_count; ++i) {
        pp_particle_t *p = &report->particles[i];
        float diameter_um = sqrtf((float)p->area_px / 3.1415926f) * 6.4f;
        size_t bin = 0;
        while (bin + 1 < PP_MAX_HIST_BINS && diameter_um > k_hist_centers_um[bin]) {
            ++bin;
        }
        report->histogram[bin]++;
        weighted += pp_allergen_weight(p->klass);
        if (report->histogram[bin] > max_bin) {
            max_bin = report->histogram[bin];
            max_bin_index = bin;
        }
    }

    report->dominant_um = k_hist_centers_um[max_bin_index];
    report->grains_per_m3 = pp_compute_concentration_grains_m3((unsigned)report->particle_count,
                                                               capture->flow_lpm,
                                                               capture->sample_seconds,
                                                               capture->sampled_area_mm2,
                                                               capture->inlet_efficiency);
    report->allergen_index = pp_compute_allergen_index(report->grains_per_m3, weighted);
}
