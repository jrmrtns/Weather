<script lang="ts">
  import { onMount } from "svelte";
  import SvgGauge from "svg-gauge";
  import debounce from 'lodash/debounce'

  let temperatureGauge;
  let gateway = `ws://${window.location.hostname}/ws`;
  //let gateway = `ws://10.10.0.9/ws`;
  let websocket: WebSocket;
  let targetTemperature = 28;

  const temperatureOptions = {
    min: 0,
    max: 50,
    dialStartAngle: 135,
    dialEndAngle: 45,
    value: 0,
    label: function (value) {
      return (Math.round(value*10)/10).toFixed(1) + " °C";
    },
    color: function (value) {
      if (value < 24) {
        return "#ef4655";
      } else if (value < 27) {
        return "#f7aa38";
      } else if (value < 33) {
        return "#5ee432";
      } else if (value < 38) {
        return "#f7aa38";
      } else {
        return "#ef4655";
      }
    },
  };

  onMount(() => {
    temperatureGauge = SvgGauge(temperatureGauge, temperatureOptions);
    initWebSocket();
  });

  const targetChange = debounce(e => {
    let data = {
      target: e.target.value
    }
    websocket.send(JSON.stringify(data));
  }, 500)

  const initWebSocket = () => {
    console.log("Trying to open a WebSocket connection...");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
  };

  const onOpen = (event) => {
    console.log("Connection opened" + event);
  };

  const onClose = (event) => {
    console.log("Connection closed" + event);
    setTimeout(initWebSocket, 5000);
  };

  const onMessage = (event) => {
    const data = JSON.parse(event.data);
    temperatureGauge.setValueAnimated(data.temperature, 1);
    targetTemperature = data.target;
  };
</script>

<style>
  main {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
    text-align: center;
    padding: 1em;
    margin: 0 auto;
    background-color: #1e2933;
    color: #b9cddf;
    padding: 0;
    margin: 0;
  }

  :global(h1) {
    color: #ff3e00;
    text-transform: uppercase;
    font-size: 4em;
    font-weight: 100;
  }

  :global(.gauge-container > .gauge > .dial) {
    stroke: #334455;
    stroke-width: 2;
  }

  :global(.gauge-container > .gauge > .value) {
    stroke: orange;
    stroke-dasharray: none;
    stroke-width: 6;
  }

  @media (min-width: 640px) {
    main {
      max-width: none;
    }
  }

  .gauge-container {
    justify-content: center;
    max-width: 90vw;

    width: 450px;
    height: 450px;
    padding: 10px;
  }

  .gauge-container {
    height: auto;
  }

  .slidecontainer {
    width: 100%;
  }

  .slider {
    -webkit-appearance: none;
    width: 100%;
    height: 15px;
    border-radius: 5px;
    background: #334455;
    outline: none;
    border: 0;
    opacity: 0.7;
    -webkit-transition: 0.2s;
    transition: opacity 0.2s;
  }

  .slider:hover {
    opacity: 1;
  }

  .slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: orange;
    cursor: pointer;
  }

  .slider::-moz-range-thumb {
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: orange;
    cursor: pointer;
  }
</style>

<main>
  <div>
  <div id="cpuSpeed" class="gauge-container" bind:this={temperatureGauge} />
  <div class="slidecontainer">
    <input
      type="range"
      min="20"
      max="50"
      bind:value={targetTemperature}
      on:change={targetChange}
      class="slider"
      id="myRange" />
    <p>Ziel: {targetTemperature}°C</p>
  </div>
</div>
</main>
