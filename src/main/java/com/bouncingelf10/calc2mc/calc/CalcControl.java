package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsScreenAccessor;
import net.minecraft.client.KeyMapping;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.screens.options.controls.KeyBindsScreen;
import org.lwjgl.glfw.GLFW;

public class CalcControl {
    public static KeyMapping selectedCalcKey = null;

    public static void tick(Minecraft minecraft) {
        if (!CalcClient.hasFoundCalculator()) return;

        byte raw = CalcState.currentKey;
        boolean depressed = (raw & 0b10000000) != 0;
        boolean changed = raw != CalcState.previousKey;
        boolean inMenu = minecraft.screen != null;
        CalcKey activeKey = CalcKey.fromCode(raw);

        if (handleSpecialCases(activeKey, raw, depressed, changed, minecraft, inMenu)) {
            if (changed) CalcState.previousKey = raw;
            return;
        }

        if (!changed) return;
        CalcState.previousKey = raw;

        if (selectedCalcKey != null) {
            if (activeKey != CalcKey.NONE && !depressed) {
                CalcBindings.set(selectedCalcKey, activeKey);
                selectedCalcKey = null;
                if (minecraft.screen instanceof KeyBindsScreen kbs) {
                    ((KeyBindsScreenAccessor) kbs).calc2mc$getKeyBindsList().resetMappingAndUpdateButtons();
                }
            }
            return;
        }

        for (KeyMapping km : CalcBindings.allKeyMappings()) {
            CalcKey bound = CalcBindings.get(km);
            if (bound == CalcKey.NONE) continue;
            if (bound != activeKey) continue;
            KeyMapping.set(km.getDefaultKey(), !depressed);
        }
    }

    public static boolean handleSpecialCases(CalcKey activeKey, byte raw, boolean depressed, boolean changed, Minecraft minecraft, boolean inMenu) {
        if (activeKey == CalcKey.UP || activeKey == CalcKey.DOWN || activeKey == CalcKey.LEFT || activeKey == CalcKey.RIGHT) {
            if (changed) {
                CalcState.arrowHoldTicks = 0;

                if (inMenu && !depressed && minecraft.screen != null) {
                    int glfwKey = switch (activeKey) {
                        case UP -> GLFW.GLFW_KEY_UP;
                        case DOWN -> GLFW.GLFW_KEY_DOWN;
                        case LEFT -> GLFW.GLFW_KEY_LEFT;
                        case RIGHT -> GLFW.GLFW_KEY_RIGHT;
                        default -> -1;
                    };
                    if (glfwKey != -1) {
                        int scancode = GLFW.glfwGetKeyScancode(glfwKey);
                        minecraft.screen.keyPressed(glfwKey, scancode, 0);
                        minecraft.screen.keyReleased(glfwKey, scancode, 0);
                    }
                }
            }

            if (!inMenu && !depressed) {
                CalcState.arrowHoldTicks++;
                double speed = Math.min(2.0 + (CalcState.arrowHoldTicks / 20.0) * 18.0, 20.0);
                CalcState.pendingDX = switch (activeKey) {
                    case LEFT -> -speed;
                    case RIGHT -> speed;
                    default -> 0.0;
                };
                CalcState.pendingDY = switch (activeKey) {
                    case UP -> -speed;
                    case DOWN -> speed;
                    default -> 0.0;
                };
            } else if (depressed) {
                CalcState.arrowHoldTicks = 0;
                CalcState.pendingDX = 0;
                CalcState.pendingDY = 0;
            }
            return true;
        }
        if (CalcBindings.get("key.attack") == activeKey || CalcBindings.get("key.use") == activeKey) {
            if (minecraft.screen == null) return false;
            int scancode = GLFW.glfwGetKeyScancode(GLFW.GLFW_KEY_ENTER);
            minecraft.screen.keyPressed(GLFW.GLFW_KEY_ENTER, scancode, 0);
            minecraft.screen.keyReleased(GLFW.GLFW_KEY_ENTER, scancode, 0);
        }
        return false;
    }
}
