#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int valid_label(const char *s, size_t n) {
    if (!n || n>=40) return 0;
    for (size_t i=0;i<n;i++) if (!isalnum((unsigned char)s[i]) && s[i]!='_' && s[i]!='-') return 0;
    return 1;
}
int bw_parse_command(const char *line, char *label, size_t cap, double *volume) {
    if (!line || !label || cap<2 || !volume) return 0;
    const char *comma=strchr(line,','); if (!comma) return 0;
    size_t op=(size_t)(comma-line); const char *arg=comma+1; size_t n=strlen(arg);
    if ((op==6 && !strncmp(line,"sample",6)) || (op==4 && !strncmp(line,"mark",4))) {
        if (!valid_label(arg,n) || n>=cap) return 0;
        memcpy(label,arg,n+1); return op==6?1:3;
    }
    if (op==6 && !strncmp(line,"volume",6)) {
        char *end=NULL; double x=strtod(arg,&end);
        if (end==arg || *end || x<0.0 || x>100.0) return 0;
        *volume=x; return 2;
    }
    return 0;
}
