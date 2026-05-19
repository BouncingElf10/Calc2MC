package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsScreenAccessor;
import net.minecraft.client.KeyMapping;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.screens.options.controls.KeyBindsScreen;

public class CalcControl {
    public static KeyMapping selectedCalcKey = null;

    public static void tick(Minecraft minecraft) {
        if (!CalcClient.hasFoundCalculator()) return;

        byte raw = CalcState.currentKey;
        boolean depressed = (raw & 0b10000000) != 0;
        if (raw == CalcState.previousKey) return;
        CalcState.previousKey = raw;

        CalcKey activeKey = CalcKey.fromCode(raw);

        if (selectedCalcKey != null) {
            if (activeKey != CalcKey.NONE && !depressed) {
                CalcBindings.set(selectedCalcKey, activeKey);
                selectedCalcKey = null;
                if (minecraft.screen instanceof KeyBindsScreen kbs) {
                    ((KeyBindsScreenAccessor) kbs).calc2mc$getKeyBindsList().resetMappingAndUpdateButtons();
                }
            }
            return;
        }

        for (KeyMapping km : CalcBindings.allKeyMappings()) {
            CalcKey bound = CalcBindings.get(km);
            if (bound == CalcKey.NONE) continue;
            if (bound != activeKey) continue;

            KeyMapping.set(km.getDefaultKey(), !depressed);
        }
    }
}
