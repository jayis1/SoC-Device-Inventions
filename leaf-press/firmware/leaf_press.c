#include "leaf_press.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
bool lp_start(lp_session *s,const char *code,uint32_t now,char *out,unsigned cap){
 if(!s||!code||!out||!cap||s->active)return false;
 size_t n=strlen(code); if(!n||n>LP_CODE_MAX)return false;
 for(size_t i=0;i<n;i++) if(!isprint((unsigned char)code[i])||code[i]=='"'||code[i]=='\\')return false;
 memcpy(s->code,code,n+1);s->started_ms=now;s->last_ms=now;s->active=true;
 int k=snprintf(out,cap,"{\"type\":\"start\",\"id\":\"%s\",\"ms\":0}",s->code);
 if(k<0||(unsigned)k>=cap){s->active=false;return false;} return true;
}
bool lp_event(lp_session *s,bool end,uint32_t now,char *out,unsigned cap){
 if(!s||!out||!cap||!s->active)return false;
 uint32_t elapsed=now-s->started_ms;
 int k=snprintf(out,cap,"{\"type\":\"%s\",\"id\":\"%s\",\"ms\":%lu}",end?"end":"mark",s->code,(unsigned long)elapsed);
 if(k<0||(unsigned)k>=cap)return false;
 s->last_ms=now;if(end)s->active=false;return true;
}
