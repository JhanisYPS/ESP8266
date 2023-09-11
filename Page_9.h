const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Medição de BPM</title>
    <style>
        /* Estilos CSS para a página */
        body {
            font-family: Arial, sans-serif;
        }
        #container {
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
        }
        #chart-container {
            margin-top: 20px;
        }
        table {
            border-collapse: collapse;
            width: 100%;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: center;
        }
        th {
            background-color: #f2f2f2;
        }
        canvas {
            width: 100%;
            max-width: 800px;
            height: 300px;
            border: 1px solid #ddd;
        }
    </style>
</head>
<body>
    <div id="container">
        <h1>Medição de BPM</h1>
        <p>BPM Atual: <span id="bpm">--</span></p>
        <button onclick="startMeasure()">Iniciar Medição</button>
        <button onclick="stopMeasure()">Parar Medição</button>
        
        <!-- Gráfico -->
        <h2>Gráfico</h2>
        <canvas id="myCanvas"></canvas>
        
        <!-- Tabela para exibir dados -->
        <h2>Dados</h2>
        <table>
            <thead>
                <tr>
                    <th>Tempo</th>
                    <th>BPM (Média de 10s)</th>
                </tr>
            </thead>
            <tbody id="data-table">
                <!-- Os dados serão preenchidos via JavaScript -->
            </tbody>
        </table>
        
        <button onclick="exportCSV()">Exportar para CSV</button>
    </div>
    
    <script>
        // Variáveis para armazenar dados de BPM e tempo
        let bpmData = [];
        let startTime;
        let intervalId;
        let axis_x = 1; // Variável para controlar a posição X no gráfico
        let tableData = [];

        // Obtenha o contexto do canvas
        const canvas = document.getElementById('myCanvas');
        const context = canvas.getContext('2d');

        // Inicie o gráfico
        drawGraph();

        // Função para iniciar a medição
        function startMeasure() {
            startTime = new Date().getTime();
            // Atualiza o gráfico a cada 1 segundo
            intervalId = setInterval(getHeartRateVal, 500);
        }

        // Função para parar a medição
        function stopMeasure() {
            clearInterval(intervalId);
        }

        // Função para obter o valor real do BPM
        function getHeartRateVal() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    const bpm = parseFloat(this.responseText);
                    const currentTime = new Date().getTime() - startTime;

                    bpmData.push(bpm);
                    document.getElementById("bpm").textContent = bpm; // Atualize a exibição de BPM
                    if (bpmData.length >= 10) {
                        const averageBPM = calculateAverageBPM(bpmData);
                        tableData.push([currentTime / 1000, averageBPM]);
                        updateDataTable(); // Atualize a tabela
                        bpmData = []; // Limpe os dados de BPM para a próxima média de 10 segundos
                    }
                    updateChart(currentTime / 1000, bpm); // Atualize o gráfico
                }
            };
            xhttp.open("GET", "getHeartRate", true);
            xhttp.send();
        }

        // Função para calcular a média dos valores de BPM
        function calculateAverageBPM(data) {
            let sum = 0;
            for (const bpm of data) {
                sum += bpm;
            }
            return (sum / data.length).toFixed(2);
        }

        // Função para atualizar a tabela com os dados de BPM
        function updateDataTable() {
            const table = document.getElementById("data-table");
            const row = document.createElement("tr");
            const lastData = tableData[tableData.length - 1];
            const time = lastData[0];
            const bpm = lastData[1];

            row.innerHTML = `
                <td>${time.toFixed(2)} s</td>
                <td>${bpm}</td>
            `;

            table.appendChild(row);
        }

        // Função para atualizar o gráfico com os dados de BPM
        function updateChart(x, y) {
            if (axis_x > 300) {
                axis_x = 1;
                canvas.width = canvas.width;
                drawGraph();
            }
            const normalizedBPM = (y - 60) / (200 - 60); // Normaliza o BPM para a faixa 0-1
            const graphHeight = 150;
            const yPos = graphHeight - normalizedBPM * graphHeight;
            set_Axis_XY(x, yPos);
        }

        // Função para exportar os dados para CSV
        function exportCSV() {
            const csvData = [['Tempo (s)', 'BPM (Média de 10s)']];
            for (const data of tableData) {
                const time = data[0].toFixed(2);
                const bpm = data[1];
                csvData.push([time, bpm]);
            }

            const csvContent = "data:text/csv;charset=utf-8," + csvData.map(e => e.join(",")).join("\n");
            const encodedUri = encodeURI(csvContent);
            const link = document.createElement("a");
            link.setAttribute("href", encodedUri);
            link.setAttribute("download", "bpm_data.csv");
            link.click();
        }

        // Função para desenhar o gráfico de base
        function drawGraph() {
            context.beginPath();
            context.moveTo(1, 150);
            context.lineTo(1, 0);
            context.moveTo(300, 150);
            context.lineTo(300, 0);

            context.moveTo(0, 1);
            context.lineTo(300, 1);
            context.moveTo(0, 25);
            context.lineTo(300, 25);
            context.moveTo(0, 50);
            context.lineTo(300, 50);
            context.moveTo(0, 75);
            context.lineTo(300, 75);
            context.moveTo(0, 100);
            context.lineTo(300, 100);
            context.moveTo(0, 125);
            context.lineTo(300, 125);
            context.moveTo(0, 149);
            context.lineTo(300, 149);

            context.lineWidth = 0.5;
            context.strokeStyle = '#DCDCDC';
            context.closePath();
            context.stroke();
        }

        // Função para plotar no gráfico com ajuste de altura
        function set_Axis_XY(x, y) {
            context.lineTo(axis_x, y);
            context.strokeStyle = 'black'; // Define a cor da linha como preta
            context.lineWidth = 1; // Define a largura da linha como 1
            context.stroke();
            if (axis_x > 300) {
                axis_x = 1;
                canvas.width = canvas.width;
                drawGraph();
            }
            axis_x++;
        }
    </script>
</body>
</html>
)=====";