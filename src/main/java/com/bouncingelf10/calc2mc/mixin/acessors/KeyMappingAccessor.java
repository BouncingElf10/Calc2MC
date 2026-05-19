package com.bouncingelf10.calc2mc.mixin.acessors;

import net.minecraft.client.KeyMapping;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.gen.Accessor;

@Mixin(KeyMapping.class)
public interface KeyMappingAccessor {
    @Accessor("clickCount")
    void calc2mc$incrementClicks(int count);

    @Accessor("isDown")
    void calc2mc$setIsDown(boolean isDown);
}
