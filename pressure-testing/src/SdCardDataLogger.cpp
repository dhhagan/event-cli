#include "Particle.h"

#include "SdCardDataLogger.h"

static Logger localLog("sd.quantaq");

SdCardDataLogger::SdCardDataLogger(SdFat &sd, uint8_t csPin, SPISettings spiSettings) : sd(sd), csPin(csPin), spiSettings(spiSettings) {

}

SdCardDataLogger::~SdCardDataLogger() {
}

void SdCardDataLogger::setup() {
}

 void SdCardDataLogger::loop() {
}

void SdCardDataLogger::write(const char *msg) {
    // check to see if the file exists
    const char *name = getName();

    Serial.println(msg);

    // check to see if the file exists; if not, create it
    if (!sd.exists(name)) {
        localLog.info("FatFile does not exist. Making new file.");

        makeNewFile();

        if (curFile.isOpen()) {
            localLog.info("Closing the current FatFile");
            curFile.close();
        }
    }

    if (!curFile.isOpen()) {
        localLog.info("Opening the local FatFile for logging");
        openDataFile();
    }

    if (curFile.isOpen()) {
        int nBytes = curFile.write(msg);

        if (nBytes > 0) {
            curFile.sync();
        }
        else {
            localLog.warn("No bytes written to uSD card.");
        }
    }
    else {
        localLog.info("Failed to write to uSD card");
    }
}

const char *SdCardDataLogger::getName() {
    snprintf(nameBuf, sizeof(nameBuf), "DATA_%d%02d%02d.csv", Time.year(), Time.month(), Time.day());

    return nameBuf;
}

void SdCardDataLogger::makeNewFile() {
    const char *name = getName();

    FatFile tmpFile;

    if (!tmpFile.open(name, O_CREAT | O_RDWR | O_APPEND)) {
        localLog.error("Could not create new file %s", name);
    }
    else {
        tmpFile.write(String::format("deviceID,%s\n", System.deviceID().c_str()));
        tmpFile.write("timestamp_iso,millis,pressure_bits_1,pressure_bits_2,");
        tmpFile.write("pressure_cmh2o_1,pressure_cmh2o_2");
        tmpFile.write("\n");

        tmpFile.close();
        localLog.trace("Created new file at %s", name);
    }
}

bool SdCardDataLogger::openDataFile() {
    const char *name = getName();

    if (curFile.open(name, O_RDWR | O_AT_END | O_APPEND)) {
        return true;
    }
    else {
        localLog.warn("Could not open data file for logging");
        return false;
    }
}
