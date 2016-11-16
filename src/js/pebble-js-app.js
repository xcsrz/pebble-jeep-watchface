var DEBUG = true;

Pebble.addEventListener('ready',
    function(e) {
        if (DEBUG) console.log('JavaScript app ready and running!');
    }
);

Pebble.addEventListener("showConfiguration",
    function() {

        if (DEBUG) {
            console.log("Showing Settings...");
        }

        // No valid image data, so use data URL to generate HTML with instructions
        Pebble.openURL("http://xcsrz.com/pebble-jeep-watchface/");
    });

Pebble.addEventListener("webviewclosed",
    function(e) {
        if (DEBUG) console.log("Webview closed");
        if (e.response) {
            if (DEBUG) console.log("Settings returned: " + e.response);
            var data = JSON.parse(decodeURIComponent(e.response));
            if (DEBUG) console.log("Config data received: " + JSON.stringify(data, null, 4));

            var configObj = {
                type: data.type,
                // model: data.model,
                seconds: data.seconds,
                jeepColor: parseInt(data.jeepColor, 16),
                timeColor: parseInt(data.timeColor, 16),
                bgColor: parseInt(data.bgColor, 16),
            };
            Pebble.sendAppMessage(configObj, function() {
                if(DEBUG) console.log("Settings update: success");
            }, function(data, error) {
                if(DEBUG) console.log("Settings update: FAILED to send -> " + JSON.stringify(data, null, 4) + "\n" + + JSON.stringify(error, null, 4));
            })
        } else {
            if (DEBUG) console.log("Settings cancelled");
        }
    });