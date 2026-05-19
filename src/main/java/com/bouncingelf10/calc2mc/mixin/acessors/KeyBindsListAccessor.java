package com.bouncingelf10.calc2mc.mixin.acessors;

import net.minecraft.client.gui.components.AbstractSelectionList;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.gen.Invoker;

@Mixin(AbstractSelectionList.class)
public interface KeyBindsListAccessor {
    @Invoker("getScrollbarPosition")
    int calc2mc$getScrollbarPosition();
}