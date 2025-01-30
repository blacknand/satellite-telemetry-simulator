#include "q_output_stream.h"


inline QTextStream& qStdout()
{
    static QTextStream r{stdout};
    return r;
}

