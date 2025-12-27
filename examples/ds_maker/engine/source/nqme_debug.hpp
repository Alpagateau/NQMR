#ifndef NQME_DEBUG_HPP 
#define NQME_DEBUG_HPP

#include <nds.h>
#define DEBUG_PRINT(fmt, ...) \
    printf("[DEBUG] %s:%d : " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
//#define printf(args...) fprintf(stderr, ##args)

#define CONSOLE_RESET   "\033[0m"
#define CONSOLE_BLACK   "\033[30m"      /* Black */
#define CONSOLE_RED     "\033[31m"      /* Red */
#define CONSOLE_GREEN   "\033[32m"      /* Green */
#define CONSOLE_YELLOW  "\033[33m"      /* Yellow */
#define CONSOLE_BLUE    "\033[34m"      /* Blue */
#define CONSOLE_MAGENTA "\033[35m"      /* Magenta */
#define CONSOLE_CYAN    "\033[36m"      /* Cyan */
#define CONSOLE_WHITE   "\033[37m"      /* White */
#define CONSOLE_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define CONSOLE_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define CONSOLE_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define CONSOLE_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define CONSOLE_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define CONSOLE_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define CONSOLE_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define CONSOLE_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


namespace NQME 
{

namespace DEBUG
{

}

}

#endif
