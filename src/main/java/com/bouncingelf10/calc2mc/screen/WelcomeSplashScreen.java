package com.bouncingelf10.calc2mc.screen;

import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.GuiGraphics;
import net.minecraft.client.gui.screens.Screen;
import net.minecraft.network.chat.Component;
import net.minecraft.util.FastColor;

public class WelcomeSplashScreen extends Screen {
    private float alpha = 0f;
    private int ticksOpen = 0;
    private static final int FADE_TICKS = 25;

    public WelcomeSplashScreen() {
        super(Component.literal("Calc Setup"));
    }

    @Override
    protected void init() {

    }

    @Override
    public void tick() {
        ticksOpen++;
        alpha = Math.min(1f, (float) ticksOpen / FADE_TICKS);
    }

    @Override
    public void render(GuiGraphics guiGraphics, int mouseX, int mouseY, float delta) {
        int onyx = FastColor.ARGB32.colorFromFloat(alpha, 10 / 255f, 10 / 255f, 10 / 255f);
        int silver = FastColor.ARGB32.colorFromFloat(alpha, 181 / 255f, 181 / 255f, 181 / 255f);
        int alabaster = FastColor.ARGB32.colorFromFloat(alpha, 230 / 255f, 230 / 255f, 230 / 255f);
        int graphite = FastColor.ARGB32.colorFromFloat(alpha, 56 / 255f, 56 / 255f, 56 / 255f);
        int grey = FastColor.ARGB32.colorFromFloat(alpha, 97 / 255f, 97 / 255f, 97 / 255f);

        guiGraphics.fill(0, 0, guiGraphics.guiWidth(), guiGraphics.guiHeight(), onyx);
        guiGraphics.drawString(Minecraft.getInstance().font,"yo this is me", 10, 10, alabaster);
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