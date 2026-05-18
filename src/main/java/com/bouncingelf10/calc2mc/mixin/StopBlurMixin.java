package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.screen.CalcSetupScreen;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.screens.Screen;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(Screen.class)
public class StopBlurMixin {
    @Inject(method = "renderBackground", at = @At(value = "INVOKE", target = "Lnet/minecraft/client/gui/screens/Screen;renderBlurredBackground(F)V"), cancellable = true)
    private void atEndOfSplash(CallbackInfo info) {
        if (Minecraft.getInstance().screen instanceof CalcSetupScreen) {
            info.cancel();
        }
    }
}
