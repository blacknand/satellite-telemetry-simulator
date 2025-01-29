/**
 * @file satellite_sim/satellite_factory.cpp
 * @brief This file is the abstract factory implementation for the satellite simulator
 * @paragraph
 * This file is the abstract factory implementation for the satellite simulator which will be overriden
 * at runtime by the compiler so the Qt app (satellite_sim) can use the Pico SDK specific implementation
 * without actually having to build for the Pico SDK specific headers. This is done via dependency injection.
 * Please refer to the UML diagram in the README.md file.
 */


#include "q_satellite_factory.h"  


SatelliteData SatFactory::get_satellite_data() {
    return satData->get_satellite_data();
}