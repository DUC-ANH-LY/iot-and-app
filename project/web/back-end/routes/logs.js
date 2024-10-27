const express = require("express");
const router = express.Router();
const Log = require("../models/logs"); // Import the Log model

// Create a new log entry
router.post("/logs", async (req, res) => {
  try {
    const { deviceName, deviceStatus, deviceMessage, deviceDuration } =
      req.body;
    const newLog = new Log({
      deviceName,
      deviceStatus,
      deviceMessage,
      deviceDuration,
    });
    await newLog.save();
    res.status(201).json(newLog);
  } catch (error) {
    res.status(400).json({ message: error.message });
  }
});

// Read all log entries
router.get("/logs", async (req, res) => {
  try {
    const logs = await Log.find();
    res.json(logs);
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
});

// Read a single log entry by ID
router.get("/logs/:id", async (req, res) => {
  try {
    const log = await Log.findById(req.params.id);
    if (!log) return res.status(404).json({ message: "Log not found" });
    res.json(log);
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
});

// Update a log entry by ID
router.put("/logs/:id", async (req, res) => {
  try {
    const { deviceName, deviceStatus, deviceMessage } = req.body;
    const updatedLog = await Log.findByIdAndUpdate(
      req.params.id,
      { deviceName, deviceStatus, deviceMessage },
      { new: true, runValidators: true }
    );
    if (!updatedLog) return res.status(404).json({ message: "Log not found" });
    res.json(updatedLog);
  } catch (error) {
    res.status(400).json({ message: error.message });
  }
});

// Delete a log entry by ID
router.delete("/logs/:id", async (req, res) => {
  try {
    const log = await Log.findByIdAndDelete(req.params.id);
    if (!log) return res.status(404).json({ message: "Log not found" });
    res.json({ message: "Log deleted successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
});

module.exports = router;
