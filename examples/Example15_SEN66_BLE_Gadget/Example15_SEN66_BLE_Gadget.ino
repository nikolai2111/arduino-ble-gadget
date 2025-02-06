// Please install the Sensirion I2C Arduino library for the SEN55 sensor module,
// before using this example code:
// https://github.com/Sensirion/arduino-i2c-sen6x
#include "Sensirion_Gadget_BLE.h"
#include <SensirionI2cSen66.h>

SensirionI2cSen66 sen6x;

// GadgetBle workflow
static int64_t lastMeasurementTimeMs = 0;
static int measurementIntervalMs = 1000;
NimBLELibraryWrapper lib;
DataProvider provider(lib, DataType::T_RH_CO2_VOC_NOX_PM25);

void printModuleVersions() {
    uint16_t error;
    char errorMessage[256];

    unsigned char productName[32];
    uint8_t productNameSize = 32;

    error = sen6x.getProductName(productName, productNameSize);

    if (error) {
        Serial.print("Error trying to execute getProductName(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("ProductName:");
        Serial.println((char*)productName);
    }

    // uint8_t firmwareMajor;
    // uint8_t firmwareMinor;
    // bool firmwareDebug;
    // uint8_t hardwareMajor;
    // uint8_t hardwareMinor;
    // uint8_t protocolMajor;
    // uint8_t protocolMinor;

    // error = sen6x.getVersion(firmwareMajor, firmwareMinor, firmwareDebug,
    //                          hardwareMajor, hardwareMinor, protocolMajor,
    //                          protocolMinor);
    // if (error) {
    //     Serial.print("Error trying to execute getVersion(): ");
    //     errorToString(error, errorMessage, 256);
    //     Serial.println(errorMessage);
    // } else {
    //     Serial.print("Firmware: ");
    //     Serial.print(firmwareMajor);
    //     Serial.print(".");
    //     Serial.print(firmwareMinor);
    //     Serial.print(", ");

    //     Serial.print("Hardware: ");
    //     Serial.print(hardwareMajor);
    //     Serial.print(".");
    //     Serial.println(hardwareMinor);
    // }
}

void printSerialNumber() {
    uint16_t error;
    char errorMessage[256];
    unsigned char serialNumber[32];
    uint8_t serialNumberSize = 32;

    error = sen6x.getSerialNumber(serialNumber, serialNumberSize);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("SerialNumber:");
        Serial.println((char*)serialNumber);
    }
}

void setup() {

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    // Initialize the GadgetBle Library
    provider.begin();
    Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
    Serial.println(provider.getDeviceIdString());

    Wire.begin(6, 7);

    uint16_t error;
    char errorMessage[256];

    sen6x.begin(Wire, 0x6B);

    error = sen6x.deviceReset();
    if (error) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    // Print SEN55 module information
    printSerialNumber();
    printModuleVersions();

    // Start Measurement
    error = sen6x.startContinuousMeasurement();

    if (error) {
        Serial.print("Error trying to execute startMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
}

void loop() {
    if (millis() - lastMeasurementTimeMs >= measurementIntervalMs) {
        measure_and_report();
    }

    provider.handleDownload();
    delay(20);
}

void measure_and_report() {
    uint16_t error;
    char errorMessage[256];

    // Read Measurement
    float massConcentrationPm1p0;
    float massConcentrationPm2p5;
    float massConcentrationPm4p0;
    float massConcentrationPm10p0;
    float ambientHumidity;
    float ambientTemperature;
    float vocIndex;
    float noxIndex;
    uint16_t co2;

    error = sen6x.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
        noxIndex, co2);

    if (error) {
        Serial.print("Error trying to execute "
                     "readMeasuredValues(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("MassConcentrationPm1p0:");
        Serial.print(massConcentrationPm1p0);
        Serial.print("\t");
        Serial.print("MassConcentrationPm2p5:");
        Serial.print(massConcentrationPm2p5);
        Serial.print("\t");
        Serial.print("MassConcentrationPm4p0:");
        Serial.print(massConcentrationPm4p0);
        Serial.print("\t");
        Serial.print("MassConcentrationPm10p0:");
        Serial.print(massConcentrationPm10p0);
        Serial.print("\t");
        Serial.print("AmbientHumidity:");
        Serial.print(ambientHumidity);
        Serial.print("\t");
        Serial.print("AmbientTemperature:");
        Serial.print(ambientTemperature);
        Serial.print("\t");
        Serial.print("CO2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("VocIndex:");
        Serial.print(vocIndex);
        Serial.print("\t");
        Serial.print("NoxIndex:");
        if (isnan(noxIndex)) {
            Serial.println("n/a");
            noxIndex = 0.0;
        } else {
            Serial.println(noxIndex);
        }
    }

    provider.writeValueToCurrentSample(ambientTemperature,
                                       SignalType::TEMPERATURE_DEGREES_CELSIUS);
    provider.writeValueToCurrentSample(
        ambientHumidity, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
    provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
    provider.writeValueToCurrentSample(vocIndex, SignalType::VOC_INDEX);
    provider.writeValueToCurrentSample(noxIndex, SignalType::NOX_INDEX);
    provider.writeValueToCurrentSample(
        massConcentrationPm2p5, SignalType::PM2P5_MICRO_GRAMM_PER_CUBIC_METER);

    provider.commitSample();
    lastMeasurementTimeMs = millis();
}
