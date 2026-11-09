#include "logger.h"
#include <assert.h>
#include <string.h>
int main(void) {
 char label[40]; double v=-1;
 assert(bw_parse_command("sample,acid",label,sizeof label,&v)==1 && !strcmp(label,"acid"));
 assert(bw_parse_command("mark,endpoint",label,sizeof label,&v)==3 && !strcmp(label,"endpoint"));
 assert(bw_parse_command("volume,12.375",label,sizeof label,&v)==2 && v>12.374 && v<12.376);
 assert(!bw_parse_command("volume,-1",label,sizeof label,&v));
 assert(!bw_parse_command("sample,bad label",label,sizeof label,&v));
 assert(!bw_parse_command("volume,1x",label,sizeof label,&v));
 return 0;
}
