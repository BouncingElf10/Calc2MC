package com.bouncingelf10.calc2mc;

import com.bouncingelf10.calc2mc.calc.CalcClient;
import net.fabricmc.api.ClientModInitializer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Calc2MCClient implements ClientModInitializer {
	public static final String MOD_ID = "calc2mc";
	public static final Logger LOGGER = LoggerFactory.getLogger(MOD_ID);

	@Override
	public void onInitializeClient() {
		LOGGER.info("Hello Fabric world!");

		CalcClient.spawnThreadToFindCalculator();
	}
}