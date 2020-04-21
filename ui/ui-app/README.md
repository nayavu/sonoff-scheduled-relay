# UI app

A simple Angular-based web UI for the remote controller

## Build

To build the UI, you need npm installed on your system. Run `npm install` to get all application dependencies and then run the build `npm run build`.

It will compile, minimize and gzip UI files into `./dist/minimized` directory, which will be used further by `platformio` to build the SPIFFS for the device. 