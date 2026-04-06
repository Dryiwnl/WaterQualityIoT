# 🌊 Monitoramento de Qualidade da Água (Meia Praia) - IoT

![Status](https://img.shields.io/badge/Status-Em_Desenvolvimento-yellow?style=for-the-badge)
![Hardware](https://img.shields.io/badge/Hardware-ESP32-blue?style=for-the-badge)
![Frontend](https://img.shields.io/badge/Frontend-HTML_|_CSS_|_JS-orange?style=for-the-badge)
![Database](https://img.shields.io/badge/Database-Firebase-yellow?style=for-the-badge)

Sistema de monitoramento inteligente para dutos de água/esgoto costeiro. Desenvolvido como projeto para o curso de Engenharia de Computação (UFSC Araranguá), o sistema coleta dados físico-químicos da água e os exibe em um Dashboard Web dinâmico e responsivo.

---

## 🎯 Funcionalidades

* **Leitura Sensorial:** Coleta de dados de pH, Turbidez e Nível/Vazão (Sensor Ultrassônico).
* **Eficiência Energética:** Implementação de `Deep Sleep` no ESP32, garantindo viabilidade para uso com baterias ao acordar em intervalos programados (1h).
* **Transmissão Segura:** Envio dos dados via requisição (HTTPS) no formato JSON para o Firebase Realtime Database.
* **Dashboard Interativo:** Interface Web com velocímetros (Gauges) em tempo real e gráfico de proporção diária.

## 🛠️ Tecnologias Utilizadas

### 💻 Embarcados (Firmware)
* Placa: ESP32 (NodeMCU)
* Linguagem: C++ / Arduino Framework
* Ambiente: VS Code com extensão PlatformIO
* Simulador: Wokwi

### 🌐 Web & Nuvem
* Frontend: HTML5, CSS3, JavaScript (Vanilla)
* Banco de Dados: Firebase Realtime Database
* Bibliotecas Gráficas: Chart.js & Canvas-Gauges

---

## 🔋 Arquitetura de Energia (Deep Sleep)

Para garantir a autonomia do hardware operando em bateria, o sistema **não** utiliza o tradicional `void loop()`. Toda a lógica principal foi estruturada no `void setup()`. 

**O ciclo de vida do microcontrolador é:**
1. Desperta do *Deep Sleep* pelo timer interno do RTC.
2. Conecta ao Wi-Fi.
3. Efetua a leitura dos sensores de qualidade e vazão.
4. Transmite o pacote JSON para a nuvem.
5. Configura o alarme para a próxima hora e entra em suspensão profunda (`esp_deep_sleep_start()`), reduzindo o consumo drasticamente.

---

## ⚙️ Como executar o projeto

### 1. Configurando o Banco de Dados (Firebase)
1. Crie um projeto no [Firebase Console](https://console.firebase.google.com/).
2. Ative o **Realtime Database**.
3. Em *Regras* (Rules), defina como `.read: true` e `.write: true`.
4. Copie a URL do seu banco de dados.

### 2. Configurando o Hardware (ESP32)
1. Abra a pasta do projeto no VS Code (PlatformIO).
2. No arquivo `src/main.cpp`, insira as credenciais da sua rede Wi-Fi e cole a URL do seu Firebase na variável `url_banco` (certifique-se de adicionar `/leituras.json` no final).
3. Compile e faça o *upload* para a placa física ou inicie o simulador Wokwi.

### 3. Rodando o Dashboard
1. Abra o arquivo `script.js` dentro da pasta `dashboardweb`.
2. Substitua o objeto `firebaseConfig` com as chaves de API do seu projeto Firebase.
3. Abra o arquivo `index.html` em seu navegador web preferido (recomendado usar a extensão *Live Server*).

---

## 👨‍💻 Desenvolvedor
**Matheus Viganó Schirrmann** *Graduando em Engenharia de Computação - Universidade Federal de Santa Catarina (UFSC)*
