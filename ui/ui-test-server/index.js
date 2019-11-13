const express = require('express');
var bodyParser = require("body-parser");
const app = express();
app.use(bodyParser.json());

const port = 3000;

var ctx = {
    config : {
        deviceName : 'deviceName',
        wifiSsid   : 'wifiSsid',
        wifiPass   : 'wifiPass',
        ntpHost    : 'pool.ntp.org',
        utcOffset  : 0
    },
    devices : {
        relay : {
            config: {
                startTime: 0,
                stopTime: 0
            },
            state: {
                on: false,
                time: 'now'
            }
        }
    }
};

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.get('/config', (req, res) => {
    res.send(ctx.config);
    // res.status(404).send(''); // if a fresh installation and there's no config file in the fs
    // res.status(500).send(''); // failed to load/parse config
});

app.post('/config', (req, res) => {
    console.log(req.body);
    if (!req.body.deviceName) {
        res.status(400).send('');
        return;
    }
    ctx.config = req.body;
    res.status(200).send();
});

app.get('/relay/config', (req, res) => {
    // res.send(ctx.devices.relay.config);
    res.status(404).send(''); // if a fresh installation and there's no config file in the fs
    // res.status(500).send(''); // failed to load/parse config
});

app.post('/relay/config', (req, res) => {
    console.log(req.body);
    ctx.devices.relay.config = req.body;

    const date = new Date();
    const now = date.getHours() * 60 + date.getMinutes();
    ctx.devices.relay.state.on = now >= ctx.devices.relay.config.startTime && now <= ctx.devices.relay.config.stopTime;

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
