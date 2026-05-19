package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.mixin.acessors.KeyBindsScreenAccessor;
import net.minecraft.client.KeyMapping;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.screens.inventory.AbstractContainerScreen;
import net.minecraft.client.gui.screens.options.controls.KeyBindsScreen;
import org.lwjgl.glfw.GLFW;

public class CalcControl {
    public static KeyMapping selectedCalcKey = null;

    private static boolean attackHeld = false;
    private static boolean useHeld = false;

    public static void tick(Minecraft minecraft) {
        if (!CalcClient.hasFoundCalculator()) return;

        byte raw = CalcState.currentKey;
        boolean depressed = (raw & 0b10000000) != 0;
        boolean changed = raw != CalcState.previousKey;
        boolean inMenu = minecraft.screen != null;
        CalcKey activeKey = CalcKey.fromCode(raw);

        if (!inMenu && (attackHeld || useHeld)) {
            attackHeld = false;
            useHeld = false;
        }

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
        boolean isMouseScreen = inMenu && isMouseDrivenScreen(minecraft);

        if (activeKey == CalcKey.UP || activeKey == CalcKey.DOWN || activeKey == CalcKey.LEFT || activeKey == CalcKey.RIGHT) {

            if (changed) {
                CalcState.arrowHoldTicks = 0;

                if (inMenu && !isMouseScreen && !depressed && minecraft.screen != null) {
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

            if (isMouseScreen && !depressed) {
                CalcState.arrowHoldTicks++;
                double speed = Math.min(2.0 + (CalcState.arrowHoldTicks / 20.0) * 18.0, 20.0);

                double gs = minecraft.getWindow().getGuiScale();
                long window = minecraft.getWindow().getWindow();
                double[] cx = new double[1], cy = new double[1];
                GLFW.glfwGetCursorPos(window, cx, cy);

                double newGuiX = Math.max(0, Math.min(
                        cx[0] / gs + switch (activeKey) { case LEFT -> -speed; case RIGHT -> speed; default -> 0.0; },
                        minecraft.getWindow().getGuiScaledWidth() - 1
                ));
                double newGuiY = Math.max(0, Math.min(
                        cy[0] / gs + switch (activeKey) { case UP -> -speed; case DOWN -> speed; default -> 0.0; },
                        minecraft.getWindow().getGuiScaledHeight() - 1
                ));

                GLFW.glfwSetCursorPos(window, newGuiX * gs, newGuiY * gs);
                minecraft.screen.mouseMoved(newGuiX, newGuiY);
            } else if (!inMenu && !depressed) {
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
            }

            if (depressed) {
                CalcState.arrowHoldTicks = 0;
                CalcState.pendingDX = 0;
                CalcState.pendingDY = 0;
            }
            return true;
        }

        boolean isAttack = CalcBindings.get("key.attack") == activeKey;
        boolean isUse = CalcBindings.get("key.use") == activeKey;

        if (inMenu && isMouseScreen) {
            if (minecraft.screen == null) return false;

            long window = minecraft.getWindow().getWindow();
            double[] cx = new double[1], cy = new double[1];
            GLFW.glfwGetCursorPos(window, cx, cy);
            double gs = minecraft.getWindow().getGuiScale();
            double mx = cx[0] / gs;
            double my = cy[0] / gs;

            if (attackHeld && isAttack && depressed) {
                minecraft.screen.mouseReleased(mx, my, GLFW.GLFW_MOUSE_BUTTON_LEFT);
                attackHeld = false;
                return true;
            }
            if (useHeld && isUse && depressed) {
                minecraft.screen.mouseReleased(mx, my, GLFW.GLFW_MOUSE_BUTTON_RIGHT);
                useHeld = false;
                return true;
            }

            if (!depressed && isAttack && !attackHeld) {
                minecraft.screen.mouseClicked(mx, my, GLFW.GLFW_MOUSE_BUTTON_LEFT);
                attackHeld = true;
                return true;
            }
            if (!depressed && isUse && !useHeld) {
                minecraft.screen.mouseClicked(mx, my, GLFW.GLFW_MOUSE_BUTTON_RIGHT);
                useHeld = true;
                return true;
            }

            if (isAttack || isUse) return true;
        }

        if ((isAttack || isUse) && inMenu && !isMouseScreen) {
            if (minecraft.screen == null) return false;
            int scancode = GLFW.glfwGetKeyScancode(GLFW.GLFW_KEY_ENTER);
            minecraft.screen.keyPressed(GLFW.GLFW_KEY_ENTER, scancode, 0);
            minecraft.screen.keyReleased(GLFW.GLFW_KEY_ENTER, scancode, 0);
            return true;
        }

        return false;
    }

    private static boolean isMouseDrivenScreen(Minecraft minecraft) {
        return minecraft.screen instanceof AbstractContainerScreen;
    }
}