import wfdb
import torch
from wfdb import processing
import matplotlib.pyplot as plt
from normalizer import filt  #class of various filteration functions
import torch.nn as nn
import torch.optim as optim




# Define a função para ler os dados de ECG
def read_ecg_data(record_path):
    record = wfdb.rdrecord(record_path, channels=[0])
    signals = record.p_signal
    return signals

# Caminho para o arquivo de ECG
record_path_normal = r"mit-bih-normal-sinus-rhythm-database-1.0.0\16265"
record_path_FA = r"mit-bih-atrial-fibrillation-database-1.0.0\04015"

# Ler os dados de ECG
ecg_data_normal = read_ecg_data(record_path_normal)
ecg_data_FA = read_ecg_data(record_path_FA)

# Exibir os dados de ECG

#Printa os dois primeiros gráficos dos dados em um intervalo de tempo do exame de 500 milisegundos.
print(wfdb.plot_wfdb(record = wfdb.rdrecord(record_path_normal,sampto=500)),"\n",wfdb.plot_wfdb(record = wfdb.rdrecord(record_path_FA,sampto=500)))

#Normalização dos dados para padronização da range do sinal
#Print do frame
print(wfdb.processing.normalize_bound(ecg_data_normal , lb = 0 , ub = 1 ))
#Declaração da variável normalizada
normalized_signal_normal = wfdb.processing.normalize_bound(ecg_data_normal , lb = 0 , ub = 1 )
normalized_signal_FA = wfdb.processing.normalize_bound(ecg_data_normal , lb = 0 , ub = 1 )
#Plotagem gráfica dos dados normalizados
plt.plot(normalized_signal_FA)
plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude')
plt.show()

#Separação do set de validação e treino para a rede neural

#Verificando tamanho da gravação para determinar separação dos sets
print(ecg_data_normal.shape,'\n',ecg_data_FA.shape)

#Média das derivações:
#wfdb.processing.sigavg(record_path_normal, extension='hea' , pn_dir = None , return_df = False , start_range = - 0.05 , stop_range = 0.05 , ann_type = 'all' , start_time = 0 , stop_time = - 1 , verbose = False )

#Lista do sinal ECG normal de referência
ECG_Normal = ecg_data_normal[:,0]

#Lista do sinal ECG com FA de referência
ECG_FA = ecg_data_FA[:,0]

#Lendo comprimento do array pegando 80% do valor e arredondando o número sem casas decimais
Train_len_normal = round(len(ecg_data_normal)*0.8, None)
Train_len_FA = round(len(ecg_data_FA)*0.8, None)

#Separando Sets
ECG_Normal_Train = ECG_Normal[0:Train_len_normal]
ECG_Normal_Test = ECG_Normal[Train_len_normal:]
ECG_FA_Train = ECG_FA[0:Train_len_FA]
ECG_FA_Test = ECG_FA[Train_len_FA:]


# Definir a arquitetura da rede neural
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.fc1 = nn.Linear(9384755, 1)
        self.fc2 = nn.Linear(1, 1)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = self.fc2(x)
        return x

# Criar uma instância da rede neural
net = Net()

# Definir a função de perda e o otimizador
criterion = nn.MSELoss()
optimizer = optim.SGD(net.parameters(), lr=0.01)

# Converter os dados de treinamento em tensores do PyTorch
input_data = torch.tensor(ECG_Normal_Train, dtype=torch.float32)
target_data = torch.tensor(ECG_FA_Train, dtype=torch.float32)

# Loop de treinamento
for epoch in range(100):
    # Zerar os gradientes acumulados
    optimizer.zero_grad()

    # Forward pass (calcular as saídas da rede neural)
    output = net(input_data)

    # Calcular a perda
    loss = criterion(output, target_data)

    # Backward pass (calcular os gradientes)
    loss.backward()

    # Atualizar os pesos
    optimizer.step()

    # Imprimir a perda a cada 10 épocas
    if (epoch + 1) % 10 == 0:
        print(f"Epoch: {epoch+1}, Loss: {loss.item()}")

# Converter os dados de teste em tensores do PyTorch
test_input = torch.tensor(ECG_Normal_Test, dtype=torch.float32)

# Testar a rede neural com os dados de teste
net.eval()
test_output = net(test_input)
print("Test Output:")
print(test_output)