const mongoose = require("mongoose");

const LogSchema = new mongoose.Schema({
  deviceName: {
    type: String,
    required: true,
  },
  deviceStatus: {
    type: String,
    required: true,
    enum: ["on", "off"], // Only allows "on" or "off" as values
  },
  deviceMessage: {
    type: String,
    required: false,
  },
  deviceDuration: {
    type: String,
    required: false,
  },
  date: {
    type: Date,
    default: Date.now,
  },
});

const Log = mongoose.model("Log", LogSchema);

module.exports = Log;
