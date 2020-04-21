# UI test server

A simple web-service for testing UI. It simulates device behaviour.
To run it, install packages via `npm install` and then run the service `node index.js`.
To simulate a fresh device (i.e. there's no config yet), run `node index.js --no-settings`. 
To simulate a corrupted settings file on the device (the device was not able to load or parse settings file), run `node index.js --corrupted-settings`