var DEBUG = false;

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

        form =  '<html><head><meta name="viewport" content="width=device-width, initial-scale=1" /></head>';
        form += '<body style="font-family: sans-serif;"><h1 style="text-align: center;">JeepFace</h1>';
		form += '<label for="show_seconds">Show Seconds</label><input id="show_seconds" type=checkbox >'; 
        form += '</body></html>';

        // No valid image data, so use data URL to generate HTML with instructions
        Pebble.openURL("data:text/html," + encodeURIComponent(form));
    });

Pebble.addEventListener("webviewclosed",
    function(e) {
        if (DEBUG) console.log("Webview closed");
        if (e.response) {
            if (DEBUG) console.log("Settings returned: " + e.response);
            if (e.response == "clear") {
                // Clear button clicked, so clear the variables and storage
                image_data = null;
                chunk_status = 0;
                localStorage.imageData = image_data;
                localStorage.chunkStatus = chunk_status;
            }
        } else {
            if (DEBUG) console.log("Settings cancelled");
        }
    });