#ifndef BW_LOGGER_H
#define BW_LOGGER_H
#include <stddef.h>
int bw_parse_command(const char *line, char *label, size_t label_size, double *volume_ml);
#endif
