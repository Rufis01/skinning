#include "log.h"
#include "application.hpp"

int main(int argc, char **argv)
{
#ifndef DISABLE_LOG
    log_setLogLevel(LOG_DEBUG);
    log_init(0);
#endif

    Application application = Application();

    application.Run();

#ifndef DISABLE_LOG
    log_fini();
#endif
    return 0;
}
