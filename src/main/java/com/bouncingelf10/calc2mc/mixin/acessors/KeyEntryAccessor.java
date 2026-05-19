package com.bouncingelf10.calc2mc.mixin.acessors;

import net.minecraft.client.gui.screens.options.controls.KeyBindsList;
import net.minecraft.network.chat.Component;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.gen.Accessor;

@Mixin(KeyBindsList.KeyEntry.class)
public interface KeyEntryAccessor {
    @Accessor("name")
    Component calc2mc$getName();

    @Accessor("hasCollision")
    boolean calc2mc$hasCollision();
}