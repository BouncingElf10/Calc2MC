package com.bouncingelf10.calc2mc.mixin.acessors;

import net.minecraft.client.MouseHandler;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.gen.Accessor;
import org.spongepowered.asm.mixin.gen.Invoker;

@Mixin(MouseHandler.class)
public interface MouseHandlerAccessor {
    @Accessor("accumulatedDX")
    void calc2mc$setAccumulatedDX(double value);

    @Accessor("accumulatedDY")
    void calc2mc$setAccumulatedDY(double value);

    @Accessor("accumulatedDX")
    double calc2mc$getAccumulatedDX();

    @Accessor("accumulatedDY")
    double calc2mc$getAccumulatedDY();
}
