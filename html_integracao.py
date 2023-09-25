import torch
from flask import Flask, request, jsonify
from Rede_Neural_V1 import Net

app = Flask(__name__)

# Carregue seu modelo de IA treinado aqui
# Substitua 'seu_modelo.pth' pelo caminho para o seu modelo treinado
model = Net()  # Certifique-se de que 'Net' corresponde à classe de seu modelo
model.load_state_dict(torch.load('modelo_normal.pth'))
model.eval()  # Defina o modelo para o modo de avaliação

# Função para pré-processar os dados do gráfico e fazer a análise de arritmia
def analyze_arrhythmia(graph_data):
    # Pré-processamento dos dados (substitua por seu pré-processamento real)
    graph_data_tensor = torch.tensor(graph_data, dtype=torch.float32).unsqueeze(1)  # Adequar aos requisitos da entrada da rede
    # Executar a análise usando sua rede neural
    with torch.no_grad():
        output = model(graph_data_tensor)
    # Interpretar o resultado da análise (substitua por sua lógica real)
    if output > 0.5:
        return "Arritmia detectada"
    else:
        return "Nenhuma arritmia detectada"

# Rota para análise de arritmia
@app.route('/analyze', methods=['POST'])
def analyze():
    data = request.get_json()
    graph_data = data.get('graph_data', [])
    
    # Chame a função de análise de arritmia com os dados do gráfico
    result = analyze_arrhythmia(graph_data)
    
    return jsonify({'predicted_arrhythmia': result})

if __name__ == '__main__':
    app.run(debug=True)
