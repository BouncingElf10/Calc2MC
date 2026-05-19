package com.bouncingelf10.calc2mc.calc;

import net.minecraft.network.chat.Component;

public enum CalcKey {
    NONE(0x00, "None"),
    DOWN(0x01, "Down"),
    LEFT(0x02, "Left"),
    RIGHT(0x03, "Right"),
    UP(0x04, "Up"),
    POWER(0x0E, "^"),
    ENTER(0x09, "Enter"),
    ADD(0x0A, "+"),
    SUB(0x0B, "-"),
    MUL(0x0C, "*"),
    DIV(0x0D, "/"),
    CHS(0x11, "(-)"),
    SK_3(0x12, "3"),
    SK_6(0x13, "6"),
    SK_9(0x14, "9"),
    RPAREN(0x15, ")"),
    TAN(0x16, "Tan"),
    VARS(0x17, "Vars"),
    DECPNT(0x19, "."),
    SK_2(0x1A, "2"),
    SK_5(0x1B, "5"),
    SK_8(0x1C, "8"),
    LPAREN(0x1D, "("),
    COS(0x1E, "Cos"),
    PRGM(0x1F, "Prgm"),
    STAT(0x20, "Stat"),
    SK_0(0x21, "0"),
    SK_1(0x22, "1"),
    SK_4(0x23, "4"),
    SK_7(0x24, "7"),
    COMMA(0x25, ","),
    SIN(0x26, "Sin"),
    APPS(0x27, "Apps"),
    GRAPHVAR(0x28, "X,T,θ,n"),
    STORE(0x2A, "Sto->"),
    LN(0x2B, "Ln"),
    LOG(0x2C, "Log"),
    SQUARE(0x2D, "x^2"),
    RECIP(0x2E, "x^-1"),
    MATH(0x2F, "Math"),
    ALPHA(0x30, "Alpha"),
    GRAPH(0x31, "Graph"),
    TRACE(0x32, "Trace"),
    ZOOM(0x33, "Zoom"),
    WINDOW(0x34, "Window"),
    YEQU(0x35, "Y="),
    SK_2ND(0x36, "2nd"),
    MODE(0x37, "Mode"),
    DEL(0x38, "Delete");

    public final byte code;
    public final String label;

    CalcKey(int code, String label) {
        this.code = (byte) code;
        this.label = label;
    }

    public static CalcKey fromCode(byte code) {
        byte pressed = (byte) (code & 0x7F);
        for (CalcKey k : values()) {
            if (k.code == pressed) return k;
        }
        return NONE;
    }

    public Component displayName() {
        return Component.literal(label);
    }
}
