package com.bouncingelf10.calc2mc.calc;

import com.bouncingelf10.calc2mc.Calc2MCClient;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
import net.minecraft.client.KeyMapping;
import net.minecraft.client.Minecraft;

import java.io.*;
import java.lang.reflect.Type;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class CalcBindings {

    private static final Gson GSON = new GsonBuilder().setPrettyPrinting().create();
    private static final Path CONFIG_PATH = Path.of("config", "calc2mc_bindings.json");

    private static final Map<String, CalcKey> bindings = new LinkedHashMap<>();

    private static final Map<String, CalcKey> DEFAULTS = Map.ofEntries(
            Map.entry("key.attack", CalcKey.DEL),
            Map.entry("key.use", CalcKey.STAT),
            Map.entry("key.forward", CalcKey.GRAPHVAR),
            Map.entry("key.left", CalcKey.MATH),
            Map.entry("key.back", CalcKey.APPS),
            Map.entry("key.right", CalcKey.PRGM),
            Map.entry("key.jump", CalcKey.VARS),
            Map.entry("key.sneak", CalcKey.SK_2ND),
            Map.entry("key.sprint", CalcKey.ALPHA),
            Map.entry("key.drop", CalcKey.MODE),
            Map.entry("key.inventory", CalcKey.POWER),
            Map.entry("key.chat", CalcKey.NONE),
            Map.entry("key.playerlist", CalcKey.SQUARE),
            Map.entry("key.pickItem", CalcKey.NONE),
            Map.entry("key.command", CalcKey.NONE),
            Map.entry("key.socialInteractions", CalcKey.NONE),
            Map.entry("key.screenshot", CalcKey.NONE),
            Map.entry("key.togglePerspective", CalcKey.NONE),
            Map.entry("key.smoothCamera", CalcKey.NONE),
            Map.entry("key.fullscreen", CalcKey.NONE),
            Map.entry("key.spectatorOutlines", CalcKey.NONE),
            Map.entry("key.swapOffhand", CalcKey.DIV),
            Map.entry("key.saveToolbarActivator", CalcKey.NONE),
            Map.entry("key.loadToolbarActivator", CalcKey.NONE),
            Map.entry("key.advancements", CalcKey.NONE),
            Map.entry("key.hotbar.1", CalcKey.YEQU),
            Map.entry("key.hotbar.2", CalcKey.WINDOW),
            Map.entry("key.hotbar.3", CalcKey.ZOOM),
            Map.entry("key.hotbar.4", CalcKey.TRACE),
            Map.entry("key.hotbar.5", CalcKey.GRAPH),
            Map.entry("key.hotbar.6", CalcKey.RECIP),
            Map.entry("key.hotbar.7", CalcKey.SIN),
            Map.entry("key.hotbar.8", CalcKey.COS),
            Map.entry("key.hotbar.9", CalcKey.TAN),
            Map.entry("key.modmenu.open_menu", CalcKey.NONE)
    );

    public static boolean initialized = false;

    public static void init() {
        for (KeyMapping km : Minecraft.getInstance().options.keyMappings) {
            bindings.put(km.getName(), CalcKey.NONE);
        }
        load();
        DEFAULTS.forEach((name, key) -> bindings.merge(name, key, (saved, def) -> saved == CalcKey.NONE ? def : saved));
        initialized = true;
    }

    public static CalcKey get(KeyMapping km) {
        return bindings.getOrDefault(km.getName(), CalcKey.NONE);
    }

    public static void set(KeyMapping km, CalcKey key) {
        bindings.put(km.getName(), key);
        save();
    }

    public static void resetAll() {
        bindings.replaceAll((name, old) -> CalcKey.NONE);
        bindings.putAll(DEFAULTS);
        save();
    }

    public static List<KeyMapping> allKeyMappings() {
        return Arrays.asList(Minecraft.getInstance().options.keyMappings);
    }

    public static void save() {
        try {
            Files.createDirectories(CONFIG_PATH.getParent());
            Map<String, String> serial = new LinkedHashMap<>();
            bindings.forEach((name, key) -> serial.put(name, key.name()));
            try (Writer w = Files.newBufferedWriter(CONFIG_PATH)) {
                GSON.toJson(serial, w);
            }
        } catch (IOException e) {
            System.err.println("Failed to save calc key bindings: " + e.getMessage());
        }
    }

    public static void load() {
        if (!Files.exists(CONFIG_PATH)) return;
        try (Reader r = Files.newBufferedReader(CONFIG_PATH)) {
            Type type = new TypeToken<Map<String, String>>() {
            }.getType();
            Map<String, String> serial = GSON.fromJson(r, type);
            if (serial == null) return;
            serial.forEach((name, keyName) -> {
                try {
                    bindings.put(name, CalcKey.valueOf(keyName));
                } catch (IllegalArgumentException ignored) {
                    Calc2MCClient.LOGGER.warn("Unknown key found in config: {}:{}", name, keyName);
                }
            });
        } catch (IOException e) {
            System.err.println("Failed to load calc key bindings: " + e.getMessage());
        }
    }
}
