const express = require('express');
var bodyParser = require("body-parser");
const app = express();
app.use(bodyParser.json());

const port = 3000;

var ctx = {
    settings : {
        deviceName : 'deviceName',
        wifiSsid   : 'wifiSsid',
        wifiPass   : 'wifiPass',
        ntpHost    : 'pool.ntp.org',
        utcOffset  : 0
    },
    devices : {
        relay : {
            settings: {
                startTime: 0,
                stopTime: 0
            },
            state: {
                power: false,
                time: 'now'
            }
        }
    }
};

// run `node index.js` with these flags to simulate different device behavior

// simulate fresh installation when there's no settings file yet
const noSettings = process.argv ? process.argv.includes('--no-settings') : false;

// simulate corrupted settings file
const corruptedSettings = process.argv ? process.argv.includes('--corrupted-settings') : false;

console.log(`No settings mode: ${noSettings}, corrupted settings mode: ${corruptedSettings}`);

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.get('/settings', (req, res) => {
    if (noSettings) {
        res.status(404).send(''); // if a fresh installation and there's no settings file yet in the FS
    } else if (corruptedSettings) {
        res.status(500).send(''); // failed to load/parse settings
    } else {
        res.send(ctx.settings);
    }
});

app.post('/settings', (req, res) => {
    console.log(req.body);
    if (!req.body.deviceName) {
        res.status(400).send('');
        return;
    }
    ctx.settings = req.body;
    res.status(200).send();
});

app.get('/relay/settings', (req, res) => {
    if (noSettings) {
        res.status(404).send(''); // if a fresh installation and there's no settings file yet in the FS
    } else if (corruptedSettings) {
        res.status(500).send(''); // failed to load/parse settings
    } else {
        res.send(ctx.devices.relay.settings);
    }
});

app.post('/relay/settings', (req, res) => {
    console.log(req.body);
    ctx.devices.relay.settings = req.body;

    const date = new Date();
    const now = date.getHours() * 60 + date.getMinutes();
    ctx.devices.relay.state.power = now >= ctx.devices.relay.settings.startTime && now <= ctx.devices.relay.settings.stopTime;

    res.status(200).send();
});


app.get('/relay/state', (req, res) => {
    res.send(ctx.devices.relay.state)
});

app.post('/reboot', (req, res) => {
    console.log("Rebooting");
    res.status(200).send();
});

app.listen(port, (err) => {
    if (err) {
        return console.log('something bad happened', err)
    }

    console.log(`server is listening on ${port}`)
});
