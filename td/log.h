#ifndef __TD_LOG_H__
#define __TD_LOG_H__

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

namespace td{

    namespace log{

        void lineBreak(int lines = 1);
        void generic(string header, string message);
        void info(string str);
        void warning(string str);
        void error(string str);


    }

}

#endif // __TD_LOG_H__
