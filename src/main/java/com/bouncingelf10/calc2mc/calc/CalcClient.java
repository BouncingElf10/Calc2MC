package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.Calc2MCClient;
import com.fazecast.jSerialComm.SerialPort;

import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Stream;

public class CalcClient {
    static final int BAUD_RATE = 115200;

    static final int CONNECT_BYTE = 0x69;
    static final int CONFIRM_BYTE = 0x67;
    static final int QUIT_BYTE = 0x42;

    static volatile SerialPort calculatorPort;

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

    public static CompletableFuture<SerialPort> connectAsync(String portName) {
        return CompletableFuture.supplyAsync(() -> connectTo(portName));
    }

    public static SerialPort tryFindCOM3Calculator() {
        try {
            for (SerialPort port : getSerialPorts()) {
                if (port.getDescriptivePortName().contains("COM3")) {
                    calculatorPort = connectTo(port.getDescriptivePortName());
                    return calculatorPort;
                }
            }
        } catch (Exception e) {
            Calc2MCClient.LOGGER.error("Error while trying to find calculator: {}", e.getMessage());
        }
        return null;
    }

    public static SerialPort connectTo(String portName) {
        for (SerialPort port : getSerialPorts()) {
            if (port.getDescriptivePortName().equals(portName)) {
                calculatorPort = port;
                calculatorPort.setComPortParameters(BAUD_RATE, 8, 1, 0);
            }
        }
        if (calculatorPort == null) return null;
        calculatorPort.openPort();
        calculatorPort.setComPortTimeouts(
                SerialPort.TIMEOUT_READ_BLOCKING,
                2000,
                0
        );
        byte[] receivedByte = new byte[1];
        calculatorPort.readBytes(receivedByte, 1);
        if (receivedByte[0] == CONNECT_BYTE) {
            byte[] sendByte = new byte[]{(byte) CONFIRM_BYTE};
            calculatorPort.writeBytes(sendByte, 1);
            spawnReadLoop();
            return calculatorPort;
        }

        calculatorPort.closePort();
        return null;
    }

    public static void quitIfConnected() {
        if (calculatorPort == null) return;
        if (!calculatorPort.isOpen()) return;

        byte[] sendByte = new byte[]{(byte) QUIT_BYTE};
        calculatorPort.writeBytes(sendByte, 1);
        calculatorPort.closePort();
        calculatorPort = null;
    }

    public static void spawnReadLoop() {
        Thread thread = new Thread(() -> {
            calculatorPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 0, 0);

            while (calculatorPort != null && calculatorPort.isOpen()) {
                byte[] buf = new byte[1];
                int read = calculatorPort.readBytes(buf, 1);
                if (read > 0) {
                    CalcState.currentKey = buf[0];
                }
            }
        });
        thread.setDaemon(true);
        thread.start();
    }

    public static boolean hasFoundCalculator() {
        return calculatorPort != null;
    }

    public static List<SerialPort> getSerialPorts() {
        return List.of(SerialPort.getCommPorts());
    }

    public static List<String> getSerialPortStrings() {
        return Stream.of(SerialPort.getCommPorts()).map(SerialPort::getDescriptivePortName).toList();
    }
}
