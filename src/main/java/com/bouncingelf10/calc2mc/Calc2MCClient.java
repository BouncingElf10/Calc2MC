package com.bouncingelf10.calc2mc;

import com.bouncingelf10.calc2mc.calc.CalcBindings;
import com.bouncingelf10.calc2mc.calc.CalcClient;
import net.fabricmc.api.ClientModInitializer;
import net.fabricmc.fabric.api.client.event.lifecycle.v1.ClientLifecycleEvents;
import net.fabricmc.fabric.api.client.event.lifecycle.v1.ClientTickEvents;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Calc2MCClient implements ClientModInitializer {
	public static final String MOD_ID = "calc2mc";
	public static final Logger LOGGER = LoggerFactory.getLogger(MOD_ID);

	@Override
	public void onInitializeClient() {
		LOGGER.info("Hello Fabric world!");

		CalcClient.spawnThreadToFindCalculator();

		ClientTickEvents.START_CLIENT_TICK.register(client -> {
			if (!CalcBindings.initialized) {
				CalcBindings.init();
			}
		});

		ClientLifecycleEvents.CLIENT_STOPPING.register(client -> {
			CalcClient.quitIfConnected();
		});
	}
}