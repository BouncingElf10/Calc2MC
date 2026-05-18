package com.bouncingelf10.calc2mc.compat;

import com.bouncingelf10.calc2mc.screen.CalcSetupScreen;
import com.terraformersmc.modmenu.api.ConfigScreenFactory;
import net.minecraft.client.gui.screens.Screen;

public class CalcSetupScreenFactory implements ConfigScreenFactory<CalcSetupScreen> {
    public static CalcSetupScreenFactory INSTANCE = new CalcSetupScreenFactory();

    @Override
    public CalcSetupScreen create(Screen screen) {
        return new CalcSetupScreen();
    }
}
