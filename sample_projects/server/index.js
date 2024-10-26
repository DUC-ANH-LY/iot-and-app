const express = require("express");
const app = express();
const cors = require("cors");
const mysql2 = require("mysql2");
const mqtt = require("mqtt");
const WebSocket = require("ws");

const db = mysql2.createConnection({
  host: "localhost",
  user: "root",
  password: "",
  database: "iot",
  dateStrings: ["DATETIME"],
});

const mqttClient = mqtt.connect({
  host: "mqtt-dashboard.com",
  port: 1883,
  username: "ducanh",
  password: "ducanh172",
});
const server = require("http").createServer(app);
const wss = new WebSocket.Server({ server: server });
const bodyParser = require("body-parser");
const { log } = require("console");

db.connect((error) =>
  error
    ? console.error("Connect error:", error)
    : console.log("Connected MySQL")
);

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(cors());

wss.on("connection", (ws) => {
  console.log("Client Connected");
  ws.on("message", (message) => {
    const json = message.toString();
    // const req = JSON.parse(json);
    // const rgbValue = req.color;

    mqttClient.publish("setRgb", json);
    // db.query(
    //   `INSERT INTO action (device_id,status,time) VALUES('${json[0]}','${
    //     json[1]
    //   }','${timeNow()}')`,
    //   (err) => {}
    // );
    // mqttClient.publish("button", json[0] + "|" + json[1]);
  });
  ws.on("close", () => console.log("Client Disconnected"));
});

mqttClient.on("message", (topic, message) => {
  const json = message.toString().split("|");
  if (topic == "test") {
    console.log("test message:", message.toString());
  }
  if (topic === "sensor") {
    s;
    db.query(
      `INSERT INTO sensor (device_id, temperature, humidity, light, time) VALUES ('${
        json[0]
      }', ${json[1]}, ${json[2]}, ${json[3]},'${timeNow()}')`,
      (err) => {}
    );
    console.log(json);
  }
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(message.toString());
    }
  });
});

app.get("/sensor", (req, res) =>
  db.query("SELECT * FROM sensor LIMIT 100", (err, data) =>
    err ? console.error(err) : res.send(JSON.stringify(data))
  )
);

//app.get('/action', (req, res) => db.query("SELECT * FROM action LIMIT 20", (err, data) => (err) ? console.error(err) : res.send(JSON.stringify(data))));

app.get("/sensor/type=:type&input=:input", (req, res) =>
  db.query(
    `SELECT * FROM sensor where ${req.params.type} like '${req.params.input}'`,
    (err, data) => (err ? console.error(err) : res.send(JSON.stringify(data)))
  )
);

//app.get('/action1', (req, res) => db.query("SELECT count(*) FROM action where status like 'on'", (err, data) => (err) ? console.error(err) : res.send(JSON.stringify(data))));

app.get("/action1", (req, res) => {
  db.query(
    "SELECT count(*) as count FROM action where device_id='fan' and status like 'on'",
    (err, data) => {
      const count = data[0].count;
      res.send(count.toString());
    }
  );
});

app.get("/action2", (req, res) => {
  db.query(
    "SELECT count(*) as count FROM action where device_id='led' and status like 'on'",
    (err, data) => {
      const count = data[0].count;
      res.send(count.toString());
    }
  );
});

mqttClient.on("connect", () => {
  console.log("Connected MQTT");
  mqttClient.subscribe("test", (err) => {
    if (!err) {
      mqttClient.publish("test", "Hello mqtt");
    }
  });
  mqttClient.subscribe("sensor");
  mqttClient.subscribe("action");
});

server.listen(3000, () => {});

function timeNow() {
  const dateTime = new Date();
  time = dateTime.toTimeString().split(" ")[0];
  [month, day, year] = dateTime.toLocaleDateString().split("/");
  return year + "-" + day + "-" + month + " " + time;
}
