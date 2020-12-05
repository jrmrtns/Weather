<script lang="ts">
  import { onMount } from 'svelte';
  import SvgGauge from 'svg-gauge';

  let temperatureGauge;
  let pressureGauge;
  let humidityGauge;
  let gateway = `ws://${window.location.hostname}/ws`;
  //let gateway = `ws://10.10.0.36/ws`;
  let websocket;

  const temperatureOptions = {
    min: 0,
    max: 50,
    dialStartAngle: 135,
    dialEndAngle: 45,
    value: 0,
    label: function (value) {
      return (Math.round(value *10)/10).toFixed(1) + ' °C';
    },
/*     color: function (value) {
      if (value < 24) {
        return '#ef4655';
      } else if (value < 27) {
        return '#f7aa38';
      } else if (value < 33) {
        return '#5ee432';
      } else if (value < 38) {
        return '#f7aa38';
      } else {
        return '#ef4655';
      }
    }, */
  };

  const pressureOptions = {
    min: 950,
    max: 1050,
    dialStartAngle: 135,
    dialEndAngle: 45,
    value: 0,
    label: function (value) {
      return Math.round(value) + ' hPa';
    },
  };

  const humidityOptions = {
    min: 0,
    max: 100,
    dialStartAngle: 135,
    dialEndAngle: 45,
    value: 0,
    label: function (value) {
      return Math.round(value) + ' %';
    },
  };

  onMount(() => {
    temperatureGauge = SvgGauge(temperatureGauge, temperatureOptions);
    pressureGauge = SvgGauge(pressureGauge, pressureOptions);
    humidityGauge = SvgGauge(humidityGauge, humidityOptions);
    initWebSocket();
  });

  const initWebSocket = () => {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
  };

  const onOpen = (event) => {
    console.log('Connection opened' + event);
  };

  const onClose = (event) => {
    console.log('Connection closed' + event);
    setTimeout(initWebSocket, 5000);
  };

  const onMessage = (event) => {
    const data = JSON.parse(event.data);
    temperatureGauge.setValueAnimated(data.temperature, 1);
    pressureGauge.setValueAnimated(data.pressure, 1);
    humidityGauge.setValueAnimated(data.humidity, 1);
  };
</script>

<style>
  main {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
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

  /*:global(.gauge-container > .gauge > .value) {
 	fill: orange;
} */

  @media (min-width: 640px) {
    main {
      max-width: none;
    }
  }

  .gauge-container {
    justify-content: center;

    width: 450px;
    height: 450px;
    padding: 10px;
  }

  .gauge-container {
    height: auto;
  }
</style>

<main>
  <div id="cpuSpeed" class="gauge-container" bind:this={temperatureGauge} />
  <div id="cpuSpeed" class="gauge-container" bind:this={pressureGauge} />
  <div id="cpuSpeed" class="gauge-container" bind:this={humidityGauge} />
</main>
