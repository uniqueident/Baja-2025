namespace BB {

    typedef enum Physical {
        // 3v3
        // 5v
        PIN_3 = 3,
        // 5v
        PIN_5 = 5,
        // GND
        PIN_7 = 7,
        PIN_8 = 8,
        // GND
        PIN_10 = 10,
        PIN_11 = 11,
        PIN_12 = 12,
        PIN_13 = 13,
        // GND
        PIN_15 = 15,
        PIN_16 = 16,
        // 3v3
        PIN_18 = 18,
        PIN_19 = 19,
        // GND
        PIN_21 = 21,
        PIN_22 = 22,
        PIN_23 = 23,
        PIN_24 = 24,
        // GND
        PIN_26 = 26,
        PIN_27 = 27,
        PIN_28 = 28,
        PIN_29 = 29,
        // GND
        PIN_31 = 31,
        PIN_32 = 32,
        PIN_33 = 33,
        // GND
        PIN_35 = 35,
        PIN_36 = 36,
        PIN_37 = 37,
        PIN_38 = 38,
        // GND
        PIN_40 = 40,

    } Physical;

    static const char* Pin_To_Name(Physical pin){
        switch (pin) {

            case 3:  return "GPIO2   (SDA1)";

            case 5:  return "GPIO3   (SCL1)";

            case 7:  return "GPIO4   (GEN) ";
            case 8:  return "GPIO14  (TXD) ";

            case 10: return "GPIO15  (RXD) ";
            case 11: return "GPIO17  (GEN) ";
            case 12: return "GPIO18  (GEN) ";
            case 13: return "GPIO27  (GEN) ";

            case 15: return "GPIO22  (GEN) ";
            case 16: return "GPIO23  (GEN) ";

            case 18: return "GPIO24  (GEN) ";
            case 19: return "GPIO10  (MOSI)";

            case 21: return "GPIO9   (MISO)";
            case 22: return "GPIO25  (GEN) ";
            case 23: return "GPIO11  (SCLK)";
            case 24: return "GPIO8   (CE0) ";

            case 26: return "GPIO7   (CE1) ";
            case 27: return "ID_SD   (I2C) ";
            case 28: return "ID_SC   (I2C) ";
            case 29: return "GPIO5   (GEN) ";

            case 31: return "GPIO6   (GEN) ";
            case 32: return "GPIO12  (GEN) ";
            case 33: return "GPIO13  (GEN) ";

            case 35: return "GPIO19  (GEN) ";
            case 36: return "GPIO16  (GEN) ";
            case 37: return "GPIO26  (GEN) ";
            case 38: return "GPIO20  (GEN) ";

            case 40: return "GPIO21  (GEN) ";
            default: return "Invalid Pin";
        }
    }
}   // BB