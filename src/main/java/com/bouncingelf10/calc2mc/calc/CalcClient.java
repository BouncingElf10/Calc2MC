package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.Calc2MCClient;
import com.fazecast.jSerialComm.SerialPort;

import java.util.List;

public class CalcClient {
    static final int BAUD_RATE = 115200;

    static final int CONNECT_BYTE = 0x67;
    static final int CONFIRM_BYTE = 0x69;
    static final int QUIT_BYTE = 0x42;

    static SerialPort calculatorPort;

    public static void spawnThreadToFindCalculator() {
        Thread thread = new Thread(() -> {
            calculatorPort = tryFindCOM3Calculator();
            if (calculatorPort == null) {
                Calc2MCClient.LOGGER.warn("COM3 Default Calculator not found. This needs to be set manually.");
                return;
            }
            calculatorPort.setComPortParameters(BAUD_RATE, 8, 1, 0);
        });
        thread.start();
    }

    public static SerialPort tryFindCOM3Calculator() {
        try {
            List<SerialPort> ports = getSerialPorts();
            for (SerialPort port : ports) {
                if (port.getDescriptivePortName().contains("COM3")) {
                    return calculatorPort;
                }
            }
            Thread.sleep(1000);
        } catch (Exception e) {
            Calc2MCClient.LOGGER.error("Error while trying to find calculator: {}", e.getMessage());
        }
        return null;
    }


    public static boolean hasFoundCalculator() {
        return calculatorPort != null;
    }

    public static List<SerialPort> getSerialPorts() {
        return List.of(SerialPort.getCommPorts());
    }
}
