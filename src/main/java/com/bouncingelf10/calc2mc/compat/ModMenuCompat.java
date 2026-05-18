package com.bouncingelf10.calc2mc.compat;

import com.terraformersmc.modmenu.api.ConfigScreenFactory;
import com.terraformersmc.modmenu.api.ModMenuApi;

public class ModMenuCompat implements ModMenuApi {
    public ConfigScreenFactory<?> getModConfigScreenFactory() {
        return CalcSetupScreenFactory.INSTANCE;
    }
}
