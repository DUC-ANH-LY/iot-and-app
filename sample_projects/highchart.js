Highcharts.chart('dothi', {
    chart: {
      type: 'line',
      animation: Highcharts.svg, // don't animate in old IE
      marginRight: 10,
      events: {
        load: function () {
          // set up the updating of the chart each second
          const series1 = this.series[0];
          const series2 = this.series[1];
          setInterval(function () {
            // sửa
            // const x = (new Date()).getTime(); // current time
            const time = (new Date()); // current time
            const x = time.getTime();

            const y1 = Math.random() * 100;
            const y2 = Math.random() * 100;
            series1.addPoint([x, y1], true, true);
            series2.addPoint([x, y2], true, true);
            const latestData1 = series1.data[series1.data.length - 1].y;
            const latestData2 = series2.data[series2.data.length - 1].y;
            const latestData3 = series2.data[series2.data.length - 1].y;

            // Hiển thị dữ liệu lên thẻ h1
            const temp = document.getElementById('temp');
            const doam = document.getElementById('doam');
            const anhsang = document.getElementById('anhsang');
            const xx = document.getElementById('x');
            const xx1 = document.getElementById('x1');
            const xx2 = document.getElementById('x2');
            if (latestData1.toFixed(1) < 20) {
              //temp.style.background = '#00FF00';
              xx.style.background = '#00FF00';

            }

            else if (latestData1.toFixed(1) < 30 && latestData1.toFixed(1) >= 20)
              xx.style.background = '#FFFF00';
            else if (latestData1.toFixed(1) < 50 && latestData1.toFixed(1) >= 30)
              xx.style.background = '#33CC00';
            else if (latestData1.toFixed(1) < 70 && latestData1.toFixed(1) >= 50)
              xx.style.background = '#FF6666';
            else {
              // temp.style.background = '#FF0000'
              xx.style.background = '#FF0000'

            }
            if (latestData2.toFixed(1) < 20) {
              // doam.style.background = '#00FF00';
              xx1.style.background = '#00FF00';
            }
            else if (latestData2.toFixed(1) < 50 && latestData2.toFixed(1) >= 20)
              xx1.style.background = '#FFFF00';
            else if (latestData2.toFixed(1) < 50 && latestData2.toFixed(1) >= 30)
              xx1.style.background = '#33CC00';
            else if (latestData2.toFixed(1) < 70 && latestData2.toFixed(1) >= 50)
              xx1.style.background = '#FF6666';
            else {
              //doam.style.background = '#FF0000'
              xx1.style.background = '#FF0000'
            }
            if (latestData3.toFixed(1) < 20) {
              // doam.style.background = '#00FF00';
              xx2.style.background = '#00FF00';
            }
            //  else if ( latestData3.toFixed(1) < 50 && latestData3.toFixed(1) >=20)
            //  xx2.style.background = '#FFFF00';
            //  else if ( latestData3.toFixed(1) < 50 && latestData3.toFixed(1) >=30)
            //  xx2.style.background = '#33CC00';
            else if (latestData3.toFixed(1) < 70 && latestData3.toFixed(1) >= 50)
              xx2.style.background = '#FF6666';
            else {
              //doam.style.background = '#FF0000'
              xx2.style.background = '#FF0000'
            }
            temp.innerHTML = `${latestData1.toFixed(1)} độ`;
            doam.innerHTML = `${latestData2.toFixed(1)} %`;
            anhsang.innerHTML = `${latestData3.toFixed(1)} A`;

            // Sửa
            // addNewRowSensor("dht11", latestData2, latestData1, latestData3, x);
            addNewRowSensor("dht11", latestData2, latestData1, time.toLocaleTimeString());

          }, 2000);
        },
      }
    },

    time: {
      useUTC: false
    },

    title: {
      text: 'Đồ Thị Nhiệt Độ - Độ Ẩm'
    },

    accessibility: {
      announceNewData: {
        enabled: true,
        minAnnounceInterval: 15000,
        announcementFormatter: function (allSeries, newSeries, newPoint) {
          if (newPoint) {
            return 'New point added. Value: ' + newPoint.y;
          }
          return false;
        }
      }
    },

    xAxis: {
      title: {
        text: "Thời Gian Thực"
      },
      type: 'datetime',
      tickPixelInterval: 0
    },

    yAxis: {
      title: {
        text: 'Giá Trị'
      },
      plotLines: [{
        value: 0,
        width: 1,
        color: '#808080'
      }]
    },

    tooltip: {
      headerFormat: '<b>{series.name}</b><br/>',
      pointFormat: '{point.x:%Y-%m-%d %H:%M:%S}<br/>{point.y:.2f}'
    },
    legend: {
      enabled: false
    },

    exporting: {
      enabled: false
    },

    series: [{
      name: 'Nhiệt Độ',
      data: (function () {
        // generate an array of random data
        const data = [];
        const time = (new Date()).getTime();

        for (let i = -10; i <= 0; i += 1) {
          data.push({
            x: time + i * 1000,
            y: Math.random() * 100
          });
        }
        return data;
      }())
    }, {
      name: 'Độ Ẩm',
      data: (function () {
        // generate an array of random data
        const data = [];
        const time = (new Date()).getTime();

        for (let i = -10; i <= 0; i += 1) {
          data.push({
            x: time + i * 1000,
            y: Math.random() * 100
          });
        }
        return data;
      }())
    }],
  });
