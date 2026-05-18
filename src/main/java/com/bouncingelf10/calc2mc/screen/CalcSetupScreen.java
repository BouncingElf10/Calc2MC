package com.bouncingelf10.calc2mc.screen;

import com.bouncingelf10.calc2mc.Calc2MCClient;
import com.bouncingelf10.calc2mc.calc.CalcClient;
import com.fazecast.jSerialComm.SerialPort;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.GuiGraphics;
import net.minecraft.client.gui.components.Button;
import net.minecraft.client.gui.screens.Screen;
import net.minecraft.network.chat.Component;
import net.minecraft.util.FastColor;
import org.joml.Vector3f;

import java.util.List;

public class CalcSetupScreen extends Screen {
    private float alpha = 0f;
    private int ticksOpen = 0;

    private static final int FADE_TICKS = 25;

    private static final int PANEL_WIDTH = 400;
    private static final int HEADER_HEIGHT = 100;
    private static final int ROW_HEIGHT = 24;
    private static final int PADDING = 20;
    private static final int CONNECT_BUTTON_WIDTH = 64;
    private static final int CONNECT_BUTTON_HEIGHT = 14;
    private static final int REFRESH_BUTTON_WIDTH = 80;
    private static final int REFRESH_BUTTON_HEIGHT = 16;
    private static final int FOOTER_HEIGHT = 36;

    private static final Vector3f BACKGROUND = rgb(8, 8, 10);
    private static final Vector3f PANEL = rgb(14, 14, 18);
    private static final Vector3f HEADER = rgb(20, 20, 26);
    private static final Vector3f ROW_EVEN = rgb(18, 18, 23);
    private static final Vector3f ROW_ODD = rgb(22, 22, 28);
    private static final Vector3f BORDER = rgb(45, 45, 58);

    private static final Vector3f ACCENT = rgb(64, 220, 190);
    private static final Vector3f ACCENT_DARK = rgb(32, 110, 96);

    private static final Vector3f TEXT_PRIMARY = rgb(230, 232, 235);
    private static final Vector3f TEXT_SECONDARY = rgb(140, 143, 150);
    private static final Vector3f TEXT_MUTED = rgb(72, 74, 80);

    private static final Vector3f STATUS_SUCCESS = rgb(64, 220, 120);
    private static final Vector3f STATUS_ERROR = rgb(220, 80, 80);
    private static final Vector3f STATUS_WARNING = rgb(255, 190, 90);

    private List<String> ports = List.of();
    private Button refreshButton;
    private Button continueButton;

    private Component statusMessage = Component.empty();
    private Vector3f statusColor = TEXT_MUTED;

    public CalcSetupScreen() {
        super(Component.translatable("screen.calc2mc.calcsetup"));
    }

    @Override
    protected void init() {
        refreshPorts();

        int footerY = panelY() + panelHeight() - FOOTER_HEIGHT;

        refreshButton = new ModernButton(
                panelX() + PANEL_WIDTH - REFRESH_BUTTON_WIDTH - PADDING,
                footerY + (FOOTER_HEIGHT - REFRESH_BUTTON_HEIGHT) / 2,
                REFRESH_BUTTON_WIDTH, REFRESH_BUTTON_HEIGHT,
                Component.translatable("screen.calc2mc.refresh"),
                button -> refreshPorts()
        );

        addRenderableWidget(refreshButton);
    }

    private void refreshPorts() {
        clearWidgets();

        setStatus(Component.translatable("screen.calc2mc.refreshing"), STATUS_WARNING);

        ports = CalcClient.getSerialPortStrings();

        if (ports.isEmpty()) {
            setStatus(Component.translatable("screen.calc2mc.noports"), STATUS_ERROR);
        } else {
            setStatus(Component.translatable("screen.calc2mc.portsfound", ports.size()), STATUS_SUCCESS);
        }

        int listTop = panelY() + HEADER_HEIGHT + PADDING;

        for (int i = 0; i < ports.size(); i++) {
            String portName = ports.get(i);

            int rowY = listTop + i * ROW_HEIGHT;
            int buttonX = panelX() + PANEL_WIDTH - PADDING - CONNECT_BUTTON_WIDTH;
            int buttonY = rowY + (ROW_HEIGHT - CONNECT_BUTTON_HEIGHT) / 2;

            addRenderableWidget(new ModernButton(buttonX, buttonY, CONNECT_BUTTON_WIDTH, CONNECT_BUTTON_HEIGHT, Component.translatable("screen.calc2mc.connect"), button -> {
                try {
                    setStatus(Component.translatable("screen.calc2mc.connecting", portName), STATUS_WARNING);

                    SerialPort port = CalcClient.connectTo(portName);

                    if (port != null && port.isOpen()) {
                        setStatus(Component.translatable("screen.calc2mc.connected", portName), STATUS_SUCCESS);
                    } else {
                        setStatus(Component.translatable("screen.calc2mc.failedconnect", portName), STATUS_ERROR);
                    }
                } catch (Exception e) {
                    setStatus(Component.literal("Error: " + e.getMessage()), STATUS_ERROR);

                    Calc2MCClient.LOGGER.error("Failed to connect", e);
                }
            }));
        }

        int footerY = panelY() + panelHeight() - FOOTER_HEIGHT;

        refreshButton = new ModernButton(
                panelX() + PANEL_WIDTH - REFRESH_BUTTON_WIDTH - PADDING,
                footerY + (FOOTER_HEIGHT - REFRESH_BUTTON_HEIGHT) / 2,
                REFRESH_BUTTON_WIDTH, REFRESH_BUTTON_HEIGHT,
                Component.translatable("screen.calc2mc.refresh"),
                button -> refreshPorts()
        );
        addRenderableWidget(refreshButton);

        continueButton = new ModernButton(
                panelX() + PADDING,
                footerY + (FOOTER_HEIGHT - REFRESH_BUTTON_HEIGHT) / 2,
                110, REFRESH_BUTTON_HEIGHT,
                Component.translatable("screen.calc2mc.continue"),
                button -> Minecraft.getInstance().setScreen(null)
        );
        addRenderableWidget(continueButton);
    }

    @Override
    public void tick() {
        ticksOpen++;
        alpha = Math.min(1f, ticksOpen / (float) FADE_TICKS);
    }

    @Override
    public void render(GuiGraphics graphics, int mouseX, int mouseY, float delta) {
        graphics.fill(0, 0, width, height, color(BACKGROUND));

        int x = panelX();
        int y = panelY();
        int height = panelHeight();

        graphics.fill(x, y, x + PANEL_WIDTH, y + height, color(PANEL));
        graphics.fill(x, y, x + PANEL_WIDTH, y + 2, color(ACCENT));

        drawBorder(graphics, x, y, PANEL_WIDTH, height, color(BORDER));

        graphics.fill(x, y, x + PANEL_WIDTH, y + HEADER_HEIGHT, color(HEADER));
        graphics.fill(x, y + HEADER_HEIGHT - 1, x + PANEL_WIDTH, y + HEADER_HEIGHT, color(BORDER));

        graphics.fill(x, y, x + 3, y + HEADER_HEIGHT - 1, color(ACCENT, 0.35f));

        Minecraft minecraft = Minecraft.getInstance();

        int textX = x + 12;
        int textWidth = PANEL_WIDTH - 24;

        graphics.drawString(minecraft.font, Component.translatable("screen.calc2mc.calcnotfound"), textX, y + 10, color(TEXT_PRIMARY));

        int wrappedY = y + 24;
        wrappedY += 10;
        for (var line : minecraft.font.split(Component.translatable("screen.calc2mc.calcnotfoundinstructions.0"), textWidth)) {
            graphics.drawString(minecraft.font, line, textX, wrappedY, color(TEXT_SECONDARY));
            wrappedY += 10;
        }
        wrappedY += 10;
        for (var line : minecraft.font.split(Component.translatable("screen.calc2mc.calcnotfoundinstructions.1"), textWidth)) {
            graphics.drawString(minecraft.font, line, textX, wrappedY, color(TEXT_MUTED));

            wrappedY += 10;
        }

        int listTop = y + HEADER_HEIGHT + PADDING;
        int headerY = listTop - 14;

        graphics.drawString(minecraft.font, Component.translatable("screen.calc2mc.port"), x + PADDING + 2, headerY, color(ACCENT_DARK));

        if (ports.isEmpty()) {
            graphics.drawString(minecraft.font, Component.translatable("screen.calc2mc.noports"), x + PADDING + 4, listTop + (ROW_HEIGHT / 2) - 4, color(TEXT_MUTED));
        } else {
            for (int i = 0; i < ports.size(); i++) {
                int rowY = listTop + i * ROW_HEIGHT;
                boolean hovered = mouseX >= x && mouseX <= x + PANEL_WIDTH && mouseY >= rowY && mouseY < rowY + ROW_HEIGHT;

                graphics.fill(x + 1, rowY, x + PANEL_WIDTH - 1, rowY + ROW_HEIGHT, color(i % 2 == 0 ? ROW_EVEN : ROW_ODD));

                if (hovered) {
                    graphics.fill(x + 1, rowY, x + 3, rowY + ROW_HEIGHT, color(ACCENT));
                }

                graphics.drawString(minecraft.font, Component.literal(ports.get(i)), x + PADDING + 4, rowY + (ROW_HEIGHT - 8) / 2, hovered ? color(TEXT_PRIMARY) : color(TEXT_SECONDARY));
            }
        }

        int footerY = y + height - FOOTER_HEIGHT;

        graphics.fill(x, footerY, x + PANEL_WIDTH, footerY + 1, color(BORDER));

        // graphics.drawString(minecraft.font, Component.translatable("screen.calc2mc.selectport"), x + PADDING, footerY + (FOOTER_HEIGHT - 8) / 2, color(TEXT_MUTED));
        graphics.drawString(minecraft.font, statusMessage, x + PADDING, footerY - 14, color(statusColor));

        super.render(graphics, mouseX, mouseY, delta);
    }

    private void drawBorder(GuiGraphics graphics, int x, int y, int width, int height, int color) {
        graphics.fill(x, y, x + width, y + 1, color);
        graphics.fill(x, y + height - 1, x + width, y + height, color);
        graphics.fill(x, y, x + 1, y + height, color);
        graphics.fill(x + width - 1, y, x + width, y + height, color);
    }

    private void setStatus(Component message, Vector3f color) {
        this.statusMessage = message;
        this.statusColor = color;
    }

    private static Vector3f rgb(int r, int g, int b) {
        return new Vector3f(r / 255f, g / 255f, b / 255f);
    }

    private int color(Vector3f color) {
        return FastColor.ARGB32.colorFromFloat(alpha, color.x, color.y, color.z);
    }

    private int color(Vector3f color, float opacity) {
        return FastColor.ARGB32.colorFromFloat(opacity * alpha, color.x, color.y, color.z);
    }

    private int panelX() {
        return (width - PANEL_WIDTH) / 2;
    }

    private int panelHeight() {
        int rows = Math.max(1, ports.size());
        return HEADER_HEIGHT + (PADDING * 2 + rows * ROW_HEIGHT) + FOOTER_HEIGHT;
    }

    private int panelY() {
        return (height - panelHeight()) / 2;
    }

    @Override
    public boolean shouldCloseOnEsc() {
        return false;
    }

    @Override
    public boolean isPauseScreen() {
        return false;
    }
}