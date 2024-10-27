const express = require("express");
const bodyParser = require("body-parser");
const passport = require("passport");
const mongoose = require("mongoose");
const compression = require("compression");
const http = require("http");
const cors = require("cors");
const mqtt = require("mqtt");
const WebSocket = require("ws");
const { updateData } = require("./routes/logs");
const wss = new WebSocket.Server({ port: process.env.PORT || 8080 });
const Log = require("./models/logs"); // Import the Log model
const { log } = require("console");

const mqttClient = mqtt.connect({
  host: "mqtt-dashboard.com",
  port: 1883,
  username: "ducanh",
  password: "ducanh172",
});

require("dotenv").config();

// Instantiate express
const app = express();
app.use(compression());

// Passport Config
require("./config/passport")(passport);

// DB Config

// Connect to MongoDB
mongoose
  .connect("mongodb://localhost:27017/iot", {
    useNewUrlParser: true,
    useFindAndModify: false,
    useUnifiedTopology: true,
    useCreateIndex: true,
  })
  .then(() => console.log("MongoDB Connected"))
  .catch((err) => console.log(err));

app.use(cors());

// Express body parser
app.use("/public", express.static("public"));
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Initialize routes middleware
app.use("/api/users", require("./routes/users"));
app.use("/api/logs", require("./routes/logs"));

// websocket hanlder
wss.on("connection", function connection(ws) {
  console.log("Websocket client connected");
  updateDataF();
  ws.on("error", console.error);

  ws.on("message", function message(data) {
    mqttClient.publish("setRgb", data.toString());
  });
});

// mqtt event hanlder
mqttClient.on("message", async (topic, message) => {
  // wss.clients.forEach((client) => {
  //   if (client.readyState === WebSocket.OPEN) {
  //     client.send({something:'something'});
  //   }
  // });
  // log data
  if (topic === "updateData") {
    const updateData = JSON.parse(message.toString());
    console.log("update data " + updateData);
    // save logs to mongo
    try {
      Object.entries(updateData).forEach(async ([key, value]) => {
        var newLog;
        if (key.includes("motion")) {
          newLog = new Log({
            deviceName: "Motion Sensor",
            deviceStatus: value == 1 ? "on" : "off",
          });
        } else if (key.includes("light")) {
          newLog = new Log({
            deviceName: "Light Sensor",
            deviceStatus: value == 1 ? "on" : "off",
          });
        } else {
          newLog = new Log({
            deviceName: "Led RGB",
            deviceStatus:
              value.R == 0 && value.G == 0 && value.B == 0 ? "off" : "on",
            deviceMessage: `Red: ${value.R}, Green: ${value.G}, Blue: ${value.B}`,
            deviceDuration: value.duration,
          });
        }
        await newLog.save();

        console.log("update Data saved!");
      });
    } catch (error) {
      console.log("Error when save update data to mongo");
    }

    updateDataF();
  } else if (topic === "syncData") {
    console.log("syncData: " + message.toString());
    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send("sync|" + message.toString());
      }
    });
  }

  // update to websocket
});

async function updateDataF() {
  var updateData1;
  await Log.aggregate([
    { $match: { deviceStatus: "on" } },
    {
      $group: {
        _id: "$deviceName",
        count: { $sum: 1 },
        totalDurationHours: {
          $sum: {
            $divide: [{ $toDouble: "$deviceDuration" }, 1000 * 60 * 60],
          },
        },
      },
    },
  ])
    .then((result) => {
      updateData1 = result;
    })
    .catch((error) => console.error(error));
  console.log("updatedata: " + typeof JSON.stringify(updateData1));

  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      // client.send("update " + updateData1.toString());
      client.send("update|" + JSON.stringify(updateData1), (err) => {
        if (err) console.log("Error sending data:", err);
      });
    }
  });
}

mqttClient.on("connect", () => {
  console.log("Connected MQTT");
  mqttClient.subscribe("test", (err) => {
    if (!err) {
      mqttClient.publish("test", "Hello mqtt");
    }
  });
  mqttClient.subscribe("updateData");
  mqttClient.subscribe("syncData");
});

const PORT = process.env.PORT;

http.createServer({}, app).listen(PORT, function () {
  console.log(
    "App listening on port " + PORT + "! Go to http://localhost:" + PORT + "/"
  );
});
