package com.bouncingelf10.calc2mc.screen;

import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.GuiGraphics;
import net.minecraft.client.gui.components.Button;
import net.minecraft.network.chat.Component;
import net.minecraft.util.FastColor;
import org.joml.Vector3f;

public class ModernButton extends Button {

    private float alpha = 1f;

    private static final Vector3f BUTTON = rgb(24, 24, 30);
    private static final Vector3f BUTTON_HOVER = rgb(32, 32, 40);
    private static final Vector3f BUTTON_DISABLED = rgb(18, 18, 22);

    private static final Vector3f BORDER = rgb(50, 50, 64);
    private static final Vector3f BORDER_HOVER = rgb(64, 220, 190);

    private static final Vector3f TEXT = rgb(230, 232, 235);
    private static final Vector3f TEXT_DISABLED = rgb(90, 92, 96);

    public ModernButton(int x, int y, int width, int height, Component message, OnPress onPress) {
        super(x, y, width, height, message, onPress, DEFAULT_NARRATION);
    }

    public void setAlpha(float alpha) {
        this.alpha = alpha;
    }

    @Override
    protected void renderWidget(GuiGraphics graphics, int mouseX, int mouseY, float delta) {
        boolean hovered = isHoveredOrFocused();

        Vector3f background;
        Vector3f border;
        Vector3f text;

        if (!active) {
            background = BUTTON_DISABLED;
            border = BORDER;
            text = TEXT_DISABLED;
        } else if (hovered) {
            background = BUTTON_HOVER;
            border = BORDER_HOVER;
            text = TEXT;
        } else {
            background = BUTTON;
            border = BORDER;
            text = TEXT;
        }

        graphics.fill(getX(), getY(), getX() + width, getY() + height, color(background));

        drawBorder(graphics, getX(), getY(), width, height, color(border));

        int textX = getX() + width / 2;
        int textY = getY() + (height - 8) / 2;

        graphics.drawCenteredString(Minecraft.getInstance().font, getMessage(), textX, textY, color(text));
    }

    private void drawBorder(GuiGraphics graphics, int x, int y, int width, int height, int color) {
        graphics.fill(x, y, x + width, y + 1, color);
        graphics.fill(x, y + height - 1, x + width, y + height, color);
        graphics.fill(x, y, x + 1, y + height, color);
        graphics.fill(x + width - 1, y, x + width, y + height, color);
    }

    private static Vector3f rgb(int r, int g, int b) {
        return new Vector3f(r / 255f, g / 255f, b / 255f);
    }

    private int color(Vector3f color) {
        return FastColor.ARGB32.colorFromFloat(alpha, color.x, color.y, color.z);
    }
}