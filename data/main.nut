Script.setErrorHandler(function(message) {
	if ((typeof message) == "string") {
		Script.debugPrint(message);
	} else {
		Script.debugPrint("Unknown error.");
	}
});

// Load resource definitions
Script.include("textures.nut");
