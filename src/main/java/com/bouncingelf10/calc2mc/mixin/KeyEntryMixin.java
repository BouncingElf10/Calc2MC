package com.bouncingelf10.calc2mc.mixin;

import com.bouncingelf10.calc2mc.calc.CalcBindings;
import com.bouncingelf10.calc2mc.calc.CalcControl;
import com.bouncingelf10.calc2mc.calc.CalcKey;
import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsListAccessor;
import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsListScreenAccessor;
import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsScreenAccessor;
import com.bouncingelf10.calc2mc.mixin.acessors.KeyEntryAccessor;
import net.minecraft.ChatFormatting;
import net.minecraft.client.KeyMapping;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.GuiGraphics;
import net.minecraft.client.gui.components.Button;
import net.minecraft.client.gui.components.ContainerObjectSelectionList;
import net.minecraft.client.gui.components.events.GuiEventListener;
import net.minecraft.client.gui.narration.NarratableEntry;
import net.minecraft.client.gui.screens.options.controls.KeyBindsList;
import net.minecraft.network.chat.Component;
import org.spongepowered.asm.mixin.Final;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.Shadow;
import org.spongepowered.asm.mixin.Unique;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfoReturnable;
import com.google.common.collect.ImmutableList;

import java.util.List;

@Mixin(KeyBindsList.KeyEntry.class)
public abstract class KeyEntryMixin extends ContainerObjectSelectionList.Entry<KeyBindsList.Entry> {

    @Final @Shadow private KeyMapping key;
    @Final @Shadow private Button changeButton;
    @Final @Shadow private Button resetButton;

    @Unique private Button calcButton;
    @Unique private KeyBindsList keyBindsList;

    @Inject(method = "<init>", at = @At("TAIL"))
    private void onInit(KeyBindsList keyBindsList, KeyMapping keyMapping, Component name, CallbackInfo ci) {
        this.keyBindsList = keyBindsList;

        calcButton = Button.builder(calcLabel(CalcBindings.get(keyMapping)), btn -> {
            CalcControl.selectedCalcKey = keyMapping;
            // worst accessor chain oat
            ((KeyBindsScreenAccessor) ((KeyBindsListScreenAccessor) this.keyBindsList).calc2mc$getKeyBindsScreen()).calc2mc$getKeyBindsList().refreshEntries();
        }).bounds(0, 0, 60, 20).build();
    }

    @Inject(method = "render", at = @At("HEAD"), cancellable = true)
    private void onRender(GuiGraphics graphics, int index, int top, int left, int width, int height, int mouseX, int mouseY, boolean hovered, float delta, CallbackInfo ci) {
        int scrollbarPos = ((KeyBindsListAccessor) keyBindsList).calc2mc$getScrollbarPosition();

        int resetX = scrollbarPos - resetButton.getWidth() - 10;
        int calcX = resetX - 4 - calcButton.getWidth();
        int changeX = calcX - 5 - changeButton.getWidth();
        int y = top - 2;

        resetButton.setPosition(resetX, y);
        calcButton.setPosition(calcX, y);
        changeButton.setPosition(changeX, y);

        resetButton.render(graphics, mouseX, mouseY, delta);
        calcButton.render(graphics, mouseX, mouseY, delta);
        changeButton.render(graphics, mouseX, mouseY, delta);

        graphics.drawString(
                Minecraft.getInstance().font,
                ((KeyEntryAccessor) this).calc2mc$getName(),
                left, top + height / 2 - 4, -1
        );

        if (((KeyEntryAccessor) this).calc2mc$hasCollision()) {
            int t = changeButton.getX() - 6;
            graphics.fill(t, top - 1, t + 3, top + height, -65536);
        }

        ci.cancel();
    }

    @Inject(method = "refreshEntry", at = @At("TAIL"))
    private void onRefreshEntry(CallbackInfo ci) {
        if (calcButton == null) return;

        CalcKey bound = CalcBindings.get(key);
        if (CalcControl.selectedCalcKey == key) {
            calcButton.setMessage(Component.literal("> ").withStyle(ChatFormatting.YELLOW).append(Component.literal("???").withStyle(ChatFormatting.WHITE, ChatFormatting.UNDERLINE)).append(Component.literal(" <").withStyle(ChatFormatting.YELLOW)));
        } else {
            calcButton.setMessage(calcLabel(bound));
        }
    }

    @Inject(method = "children", at = @At("HEAD"), cancellable = true)
    private void onChildren(CallbackInfoReturnable<List<? extends GuiEventListener>> cir) {
        cir.setReturnValue(ImmutableList.of(changeButton, calcButton, resetButton));
    }

    @Inject(method = "narratables", at = @At("HEAD"), cancellable = true)
    private void onNarratables(CallbackInfoReturnable<List<? extends NarratableEntry>> cir) {
        cir.setReturnValue(ImmutableList.of(changeButton, calcButton, resetButton));
    }

    @Unique
    private static Component calcLabel(CalcKey key) {
        if (key == CalcKey.NONE) {
            return Component.literal("[None]").withStyle(ChatFormatting.DARK_GRAY);
        }
        return Component.literal("[C] ").withStyle(ChatFormatting.AQUA).append(Component.literal(key.label).withStyle(ChatFormatting.WHITE));
    }
}