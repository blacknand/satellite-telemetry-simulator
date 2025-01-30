#ifndef Q_OUTPUT_STREAM_H   
#define Q_OUTPUT_STREAM_H

#pragma once

#include <QTextStream>


inline QTextStream& qStdout()
{
    static QTextStream r{stdout};
    return r;
}

#endif // Q_OUTPUT_STREAM_H