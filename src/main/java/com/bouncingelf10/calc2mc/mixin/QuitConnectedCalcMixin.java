package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.calc.CalcClient;
import net.minecraft.client.Minecraft;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(Minecraft.class)
public class QuitConnectedCalcMixin {

	@Inject(method = "destroy", at = @At("HEAD"))
	private void onMinecraftDestroy(CallbackInfo ci) {
		CalcClient.quitIfConnected();
	}

	@Inject(method = "close", at = @At("HEAD"))
	private void onMinecraftClose(CallbackInfo ci) {
		CalcClient.quitIfConnected();
	}

	@Inject(method = "stop", at = @At("HEAD"))
	private void onMinecraftStop(CallbackInfo ci) {
		CalcClient.quitIfConnected();
	}

}
