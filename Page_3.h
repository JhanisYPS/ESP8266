const char MAIN_page[] PROGMEM = R"=====(<!DOCTYPE html>
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
        
        <!-- Gráfico para exibir dados -->
        <h2>Gráfico</h2>
        <div id="chart-container">
            <!-- O gráfico será gerado via JavaScript -->
        </div>
        
        <button onclick="exportCSV()">Exportar para CSV</button>
    </div>
    
    <script>
        // Variáveis para armazenar dados de BPM e tempo
        let bpmData = [];
        let startTime;
        let intervalId;

        // Função para iniciar a medição
        function startMeasure() {
            startTime = new Date().getTime();
            intervalId = setInterval(getHeartRateVal, 1000); // Chama getHeartRateVal a cada 1 segundo
        }

        // Função para parar a medição
        function stopMeasure() {
            clearInterval(intervalId);
        }

        // Função para obter o valor do BPM
        function getHeartRateVal() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    const bpm = parseFloat(this.responseText); // Converter resposta para número
                    if (!isNaN(bpm)) {
                        const currentTime = new Date().getTime() - startTime;
                        bpmData.push([currentTime, bpm]);
                        document.getElementById("bpm").textContent = bpm; // Atualize a exibição de BPM
                        updateDataTable(); // Atualize a tabela
                        updateChart(); // Atualize o gráfico
                    }
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
                const time = bpmData[i][0] / 1000; // Converter para segundos
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
            const data = google.visualization.arrayToDataTable(bpmData);

            const options = {
                title: "Gráfico de BPM",
                curveType: "function",
                legend: { position: "bottom" },
                vAxis: { title: "BPM" },
                hAxis: { title: "Tempo (s)" }
            };

            const chart = new google.visualization.LineChart(document.getElementById("chart-container"));
            chart.draw(data, options);
        }

        // Função para exportar os dados para CSV
        function exportCSV() {
            const csvData = [["Tempo (s)", "BPM"]];
            for (let i = 0; i < bpmData.length; i++) {
                const time = bpmData[i][0] / 1000; // Converter para segundos
                const bpm = bpmData[i][1];
                csvData.push([time.toFixed(2), bpm]);
            }

            // Crie um objeto Blob para o CSV
            const csvBlob = new Blob([csvData.map(row => row.join(",")).join("\n")], { type: "text/csv" });

            // Crie um link para download
            const csvURL = URL.createObjectURL(csvBlob);
            const link = document.createElement("a");
            link.href = csvURL;
            link.download = "bpm_data.csv";
            link.click();
        }
    </script>

    <!-- Inclua o script do Google Charts -->
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
        google.charts.load('current', {'packages':['corechart']});
        google.charts.setOnLoadCallback(updateChart);
    </script>
</body>
</html>
)=====";