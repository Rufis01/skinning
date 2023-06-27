#include "log.h"
#include "application.hpp"

int main(int argc, char **argv)
{
    log_setLogLevel(LOG_DEBUG);
    log_init(0);

    Application application = Application();

    application.Run();

    log_fini();
    return 0;
}
