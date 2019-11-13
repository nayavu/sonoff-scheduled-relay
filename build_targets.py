# define target for building UI, i.e. `pio run -t buildui` will build Angular app into `./ui/ui-app/dist/spiffs`, which then can be uploaded to the SPIFFS
Import("env")
env.AlwaysBuild(env.Alias("buildui", None, ["npm run --prefix ./ui/ui-app build"]))