package com.bouncingelf10.calc2mc.mixin.acessors;

import net.minecraft.client.gui.screens.options.controls.KeyBindsList;
import net.minecraft.client.gui.screens.options.controls.KeyBindsScreen;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.gen.Accessor;

@Mixin(KeyBindsList.class)
public interface KeyBindsListScreenAccessor {
    @Accessor("keyBindsScreen")
    KeyBindsScreen calc2mc$getKeyBindsScreen();
}
