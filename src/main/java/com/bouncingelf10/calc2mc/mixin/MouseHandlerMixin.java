package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.calc.CalcState;
import com.bouncingelf10.calc2mc.mixin.acessors.MouseHandlerAccessor;
import net.minecraft.client.MouseHandler;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(MouseHandler.class)
public class MouseHandlerMixin {
    @Inject(
            method = "handleAccumulatedMovement",
            at = @At(
                    value = "INVOKE",
                    target = "Lnet/minecraft/client/MouseHandler;turnPlayer(D)V"
            )
    )
    private void calc2mc$injectBeforeTurn(CallbackInfo ci) {
        if (CalcState.pendingDX == 0 && CalcState.pendingDY == 0) return;
        MouseHandlerAccessor self = (MouseHandlerAccessor)(Object) this;
        self.calc2mc$setAccumulatedDX(self.calc2mc$getAccumulatedDX() + CalcState.pendingDX);
        self.calc2mc$setAccumulatedDY(self.calc2mc$getAccumulatedDY() + CalcState.pendingDY);
    }
}