#include "log.h"

void td::log::lineBreak(int lines)
{
    for (int idx = 0; idx < lines; ++idx)
    {
        cout << endl;
    }
}

void td::log::generic(string header, string message)
{
    cout << header << ": " << message << endl;
}

void td::log::info(string str)
{
    generic("Info", str);
}

void td::log::warning(string str)
{
    generic("Warning", str);
}

void td::log::error(string str)
{
    generic("Error", str);
}
