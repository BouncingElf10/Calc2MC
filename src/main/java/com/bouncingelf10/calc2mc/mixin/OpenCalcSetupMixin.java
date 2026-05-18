package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.screen.WelcomeSplashScreen;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.screens.LoadingOverlay;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(LoadingOverlay.class)
public class OpenCalcSetupMixin {
	@Inject(method = "render", at = @At(value = "INVOKE", target = "Lnet/minecraft/client/gui/screens/Screen;init(Lnet/minecraft/client/Minecraft;II)V"))
	private void init(CallbackInfo info) {
		Minecraft.getInstance().execute(() -> {
			Minecraft.getInstance().setScreen(new WelcomeSplashScreen());
		});
	}
}