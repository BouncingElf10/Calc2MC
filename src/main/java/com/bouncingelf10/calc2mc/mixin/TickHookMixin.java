package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.calc.CalcControl;
import net.minecraft.client.Minecraft;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(Minecraft.class)
public class TickHookMixin {
	@Inject(method = "tick", at = @At("HEAD"))
	private void onTick(CallbackInfo ci) {
		CalcControl.tick((Minecraft) (Object) this);
	}
}
