import React from "react";
// node.js library that concatenates classes (strings)
import classnames from "classnames";
// javascipt plugin for creating charts
import Chart from "chart.js";
// react plugin used to create charts
import { Line, Bar } from "react-chartjs-2";
import { SketchPicker } from "react-color";

import ButtonGroup from "react-bootstrap/ButtonGroup";
import ToggleButton from "react-bootstrap/ToggleButton";
// reactstrap components
import {
  Button,
  Card,
  CardHeader,
  CardBody,
  NavItem,
  NavLink,
  Nav,
  Progress,
  Table,
  Container,
  Row,
  Col,
} from "reactstrap";

// core components
import {
  chartOptions,
  parseOptions,
  chartExample1,
  chartExample2,
} from "variables/charts.js";

import Header from "components/Headers/Header.js";

class Index extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      noTurnLed: 0,
      noMotionDetected: 0,
      noLightDetected: 0,
      noLedDuration: 0,
      activeNav: 1,
      chartExample1Data: "data1",
      background: "#fff", // Default background color
      radios: [
        { name: "Bật", value: "1" },
        { name: "Tắt", value: "0" },
      ],
      radioValue: "1",
      listening: false, // New state to manage listening status
      command: "",
    };
    if (window.Chart) {
      parseOptions(Chart, chartOptions());
    }
    this.webSocketClient = null;
    this.recognition = null;
  }

  componentDidMount() {
    // Initialize WebSocket connection
    this.webSocketClient = new WebSocket("ws://localhost:8080");
    // WebSocket open event
    this.webSocketClient.onopen = () => {
      console.log("WebSocket is open now.");
    };

    // WebSocket message event
    this.webSocketClient.onmessage = (event) => {
      const data = event.data.split("|");
      if (data[0] == "sync") {
        const { R, G, B } = JSON.parse(data[1]).getRgb;
        if (R == 0 && G == 0 && B == 0) {
          this.setState({ radioValue: "0" });
        } else {
          this.setState({ radioValue: "1" });
        }
      } else if (data[0] == "update") {
        const updateData2 = JSON.parse(data[1]);
        updateData2.forEach((element) => {
          if (element._id.includes("Light")) {
            this.setState({ noLightDetected: element.count });
          }
          if (element._id.includes("Led")) {
            this.setState({ noTurnLed: element.count });
            this.setState({ noLedDuration: element.totalDurationHours });
          }
          if (element._id.includes("Motion"))
            this.setState({ noMotionDetected: element.count });
        });
        console.log(this.state);
      }

      this.initializeVoiceRecognition();

      // this.handleNewData(data);
    };

    // WebSocket close event
    this.webSocketClient.onclose = () => {
      console.log("WebSocket is closed now.");
    };
  }

  initializeVoiceRecognition = () => {
    // Initialize Speech Recognition
    const SpeechRecognition =
      window.SpeechRecognition || window.webkitSpeechRecognition;
    if (SpeechRecognition) {
      this.recognition = new SpeechRecognition();
      this.recognition.continuous = true;
      this.recognition.onresult = this.handleVoiceCommand;
    } else {
      console.warn("Speech Recognition API is not supported in this browser.");
    }
  };

  startListening = () => {
    if (this.recognition) {
      this.setState({ listening: true });
      this.recognition.start();
    }
  };

  stopListening = () => {
    if (this.recognition) {
      this.setState({ listening: false });
      this.recognition.stop();
    }
  };

  handleVoiceCommand = (event) => {
    const lastCommand = event.results[event.results.length - 1][0].transcript
      .trim()
      .toLowerCase();
    console.log("Voice command received:", lastCommand);
    this.setState({ command: lastCommand });

    if (lastCommand.includes("on")) {
      this.webSocketClient.send(JSON.stringify({ color: [255, 250, 250] }));
    } else if (lastCommand.includes("off")) {
      this.webSocketClient.send(JSON.stringify({ color: [0, 0, 0] }));
    } else if (lastCommand.includes("red")) {
      this.webSocketClient.send(JSON.stringify({ color: [255, 0, 0] }));
    } else if (lastCommand.includes("blue")) {
      this.webSocketClient.send(JSON.stringify({ color: [0, 0, 255] }));
    } else if (lastCommand.includes("green")) {
      this.webSocketClient.send(JSON.stringify({ color: [0, 255, 0] }));
    } else if (lastCommand.includes("yellow")) {
      this.webSocketClient.send(JSON.stringify({ color: [255, 255, 0] }));
    }
  };

  handleChangeComplete = (color) => {
    this.setState({ background: color.hex }); // Update state with the selected color
    this.webSocketClient.send(
      JSON.stringify({ color: [color.rgb.r, color.rgb.g, color.rgb.b] })
    );
    console.log("Selected color:", color.hex);
  };

  toggleNavs = (e, index) => {
    e.preventDefault();
    this.setState({
      activeNav: index,
      chartExample1Data:
        this.state.chartExample1Data === "data1" ? "data2" : "data1",
    });
  };
  render() {
    return (
      <>
        <Header data={this.state} />
        {/* Page content */}
        <Container className="mt-3" fluid>
          <Row className="mt-5">
            <Col xl="4">
              <Card className="shadow">
                <CardHeader className="border-0">
                  <Row className="align-items-center">
                    <div className="col">
                      <h3 className="mb-2">Change Led Color</h3>
                    </div>
                  </Row>
                  <SketchPicker
                    color={this.state.background}
                    onChangeComplete={this.handleChangeComplete}
                  />
                </CardHeader>
              </Card>
            </Col>
            <Col xl="4">
              <Card className="shadow">
                <CardHeader className="border-0">
                  <Row className="align-items-center">
                    <ButtonGroup>
                      {this.state.radios.map((radio, idx) => (
                        <ToggleButton
                          key={idx}
                          id={`radio-${idx}`}
                          type="radio"
                          variant={
                            idx % 2 ? "outline-danger" : "outline-success"
                          }
                          name="radio"
                          value={radio.value}
                          checked={this.state.radioValue === radio.value}
                          onChange={(e) => {
                            this.setState({
                              radioValue: e.currentTarget.value,
                            });
                            if (e.currentTarget.value === "1") {
                              this.webSocketClient.send(
                                JSON.stringify({ color: [255, 250, 250] })
                              );
                            } else {
                              this.webSocketClient.send(
                                JSON.stringify({ color: [0, 0, 0] })
                              );
                            }
                          }}
                        >
                          {radio.name}
                        </ToggleButton>
                      ))}
                    </ButtonGroup>
                    <Row className="mt-2 ml-1">
                      <Button
                        onClick={this.startListening}
                        disabled={this.state.listening}
                      >
                        Start Voice Control
                      </Button>
                      <Button
                        onClick={this.stopListening}
                        disabled={!this.state.listening}
                      >
                        Stop Voice Control
                      </Button>
                      <textarea
                        style={{ marginTop: "20px" }}
                        value={this.state.command}
                        rows={4}
                        cols={40}
                      ></textarea>
                    </Row>
                  </Row>
                </CardHeader>
              </Card>
            </Col>
          </Row>
        </Container>
      </>
    );
  }
}

export default Index;
