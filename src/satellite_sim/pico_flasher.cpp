#include "pico_flasher.h"


void FlashPicoUf2File::flashUf2File() {
    // /Users/nathanblackburn/.pico-sdk/ninja/v1.12.1/ninja -C /Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite-telemetry-simulator/build
    QString command = "/Users/nathanblackburn/.pico-sdk/picotool/2.1.0/picotool/picotool";
    QStringList arguments = { "load", "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/satellite-telemetry-simulator/build/satellite-telemetry-simulator.elf", "-fx" };
    process.start(command, arguments);
}


void FlashPicoUf2File::handleUf2FileFlashed(const QProcess::ExitStatus &status) {
    if (status == QProcess::NormalExit) {
        emit uf2FileFlashed(status);
    }
}


void FlashPicoUf2File::handleUf2FileError(const QProcess::ProcessError &error) {
    emit errorOccurred(error);
}