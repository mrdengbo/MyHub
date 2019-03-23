#include <iostream>

#include "timstamp.h"
#include "singleton.h"
#include "log4cpphelper.h"

int main(int argc, char ** argv[])
{
    int i = 0;

    // LOG_INFO("test info"); 
    LOG_INFO_ARGS("%s [%d] test %d %s", __FILE__, __LINE__ , 5, "linux");
    LOG_NOTICE("test NOTICE"); 
    LOG_WARN("test warn"); 
    LOG_ERROR("test error"); 
    LOG_CRIT("test crit"); 
    LOG_ALERT("test alter");

    getchar();
	return 0;
}