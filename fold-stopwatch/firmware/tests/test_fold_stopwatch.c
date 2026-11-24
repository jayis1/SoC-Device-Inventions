#include "fold_stopwatch.h"
#include <assert.h>
int main(void) {
 fs_state_t s; fs_event_t e; fs_init(&s);
 assert(fs_edge(&s,0,100,&e) && e.sequence==1 && e.elapsed_us==0);
 assert(!fs_edge(&s,0,200,&e));
 assert(fs_edge(&s,1,350,&e) && e.sequence==2 && e.elapsed_us==250);
 assert(!fs_edge(&s,0,300,&e));
 assert(fs_edge(&s,0,FS_MAX_GAP_US+1000,&e)==false);
 assert(fs_edge(&s,1,FS_MAX_GAP_US+1100,&e) && e.elapsed_us==0);
 assert(!fs_edge(&s,2,FS_MAX_GAP_US+1200,&e));
 return 0;
}
