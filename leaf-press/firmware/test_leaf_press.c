#include "leaf_press.h"
#include <assert.h>
#include <string.h>
int main(void){lp_session s={0};char b[LP_LINE_MAX];assert(lp_start(&s,"herb-7",100,b,sizeof b));assert(strstr(b,"herb-7"));assert(!lp_start(&s,"x",101,b,sizeof b));assert(lp_event(&s,false,600,b,sizeof b));assert(strstr(b,"500"));assert(lp_event(&s,true,900,b,sizeof b));assert(!lp_event(&s,false,901,b,sizeof b));assert(!lp_start(&s,"bad\"id",0,b,sizeof b));return 0;}
