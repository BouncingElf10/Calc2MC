package com.bouncingelf10.calc2mc.calc;

public class CalcState {
    public static volatile byte currentKey = 0;
    public static volatile byte previousKey = 0;
    public static int arrowHoldTicks = 0;
    public static double pendingDX = 0;
    public static double pendingDY = 0;
}
