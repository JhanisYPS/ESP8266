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
        
        <!-- Tabela para exibir dados -->
        <h2>Dados</h2>
        <table>
            <thead>
                <tr>
                    <th>Tempo</th>
                    <th>BPM</th>
                </tr>
            </thead>
            <tbody id="data-table">
                <!-- Os dados serão preenchidos via JavaScript -->
            </tbody>
        </table>
        
        <!-- Gráfico -->
        <h2>Gráfico</h2>
        <canvas id="myCanvas"></canvas>
        
        <button onclick="exportCSV()">Exportar para CSV</button>
    </div>
    
    <script>
        // Variáveis para armazenar dados de BPM e tempo
        let bpmData = [];
        let startTime;
        let intervalId;
        let axis_x = 1; // Variável para controlar a posição X no gráfico

        // Obtenha o contexto do canvas
        const canvas = document.getElementById('myCanvas');
        const context = canvas.getContext('2d');

        // Inicie o gráfico
        drawGraph();

        // Função para iniciar a medição
        function startMeasure() {
            startTime = new Date().getTime();
            intervalId = setInterval(getHeartRateVal, 10000); // Chama getHeartRateVal a cada 10 segundos
        }

        // Função para parar a medição
        function stopMeasure() {
            clearInterval(intervalId);
        }

        // Função para obter o valor do BPM do servidor
        function getHeartRateVal() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    const bpm = parseFloat(this.responseText);
                    const currentTime = new Date().getTime() - startTime;

                    bpmData.push([currentTime / 1000, bpm]);
                    document.getElementById("bpm").textContent = bpm; // Atualize a exibição de BPM
                    updateDataTable(); // Atualize a tabela
                    updateChart(); // Atualize o gráfico
                }
            };
            xhttp.open("GET", "getHeartRate", true);
            xhttp.send();
        }

        // Função para atualizar a tabela com os dados de BPM
        function updateDataTable() {
            const table = document.getElementById("data-table");
            table.innerHTML = ""; // Limpe a tabela

            for (let i = 0; i < bpmData.length; i++) {
                const row = document.createElement("tr");
                const time = bpmData[i][0];
                const bpm = bpmData[i][1];

                row.innerHTML = `
                    <td>${time.toFixed(2)} s</td>
                    <td>${bpm}</td>
                `;

                table.appendChild(row);
            }
        }

        // Função para atualizar o gráfico com os dados de BPM
        function updateChart() {
            canvas.width = canvas.width; // Limpe o canvas
            drawGraph();
            context.beginPath();
            context.moveTo(1, 150 - bpmData[0][1]);

            for (let i = 1; i < bpmData.length; i++) {
                const time = bpmData[i][0];
                const bpm = bpmData[i][1];
                context.lineTo(time * 30, 150 - bpm); // Ajuste a escala do tempo e inverta a coordenada Y
            }

            context.lineWidth = 2;
            context.strokeStyle = '#0583F2';
            context.stroke();
        }

        // Função para exportar os dados para CSV
        function exportCSV() {
            const csvData = [['Tempo (s)', 'BPM']];
            for (let i = 0; i < bpmData.length; i++) {
                const time = bpmData[i][0].toFixed(2);
                const bpm = bpmData[i][1];
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
    </script>
</body>
</html>
)=====";