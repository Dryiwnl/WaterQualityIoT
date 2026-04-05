// 1. CONFIGURAÇÃO DO FIREBASE
const firebaseConfig = {
    apiKey: "COLE_SUA_API_KEY",
    authDomain: "waterquality-iot-49505.firebaseapp.com",
    databaseURL: "https://waterquality-iot-49505-default-rtdb.firebaseio.com",
    projectId: "waterquality-iot-49505",
    storageBucket: "waterquality-iot-49505.appspot.com",
    messagingSenderId: "COLE_SEU_ID",
    appId: "COLE_SEU_APP_ID"
};

firebase.initializeApp(firebaseConfig);
const db = firebase.database();

// Variáveis de acumulação
let totalBoas = 0;
let totalRuins = 0;

// 2. DATA ATUAL
const opcoesData = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };
document.getElementById('data-atual').innerText = new Date().toLocaleDateString('pt-BR', opcoesData);

// 3. GRÁFICO DE PIZZA
const ctxPizza = document.getElementById('graficoPizza').getContext('2d');
const graficoPizza = new Chart(ctxPizza, {
    type: 'pie',
    data: {
        labels: ['Boa', 'Ruim'],
        datasets: [{
            data: [0, 0],
            backgroundColor: ['#34d399', '#f87171'],
            borderWidth: 0
        }]
    },
    options: { 
        responsive: true, 
        plugins: { legend: { labels: { color: '#94a3b8' } } } 
    }
});

// 4. VELOCÍMETROS
const commonConfig = { 
    width: 220, height: 220, colorPlate: "#1e293b", colorNumbers: "#f8fafc", 
    needleType: "arrow", colorNeedle: "#38bdf8", animationDuration: 1000 
};
var gaugePh = new RadialGauge({ ...commonConfig, renderTo: 'gauge-ph', maxValue: 14, majorTicks: ['0','2','4','6','8','10','12','14'] }).draw();
var gaugeTurb = new RadialGauge({ ...commonConfig, renderTo: 'gauge-turbidez', maxValue: 1000, majorTicks: ['0','250','500','750','1000'] }).draw();
var gaugeDist = new RadialGauge({ ...commonConfig, renderTo: 'gauge-distancia', maxValue: 400, majorTicks: ['0','100','200','300','400'] }).draw();

// 5. ESCUTA EM TEMPO REAL
db.ref('leituras').on('value', (snapshot) => {
    const data = snapshot.val();
    if (!data) return;

    // Atualiza gauges e hora
    gaugePh.value = data.pH;
    gaugeTurb.value = data.turbidez;
    gaugeDist.value = data.distancia;
    const agora = new Date();
    document.getElementById('hora-coleta').innerText = agora.getHours() + ":" + String(agora.getMinutes()).padStart(2, '0');

    // LÓGICA DE ACUMULAÇÃO ROBUSTA
    // Converte para string, remove espaços e coloca em minúsculo para comparar
    let alertaString = String(data.alerta_esgoto).trim().toLowerCase();

    if (alertaString === "true") {
        totalRuins++;
        console.log("LOG: Leitura RUIM detectada.");
    } else {
        totalBoas++;
        console.log("LOG: Leitura BOA detectada.");
    }

    // Atualiza a Pizza
    graficoPizza.data.datasets[0].data = [totalBoas, totalRuins];
    graficoPizza.update();

    // Atualiza o Banner de Alerta
    const boxAlerta = document.getElementById('statusAlerta');
    if (alertaString === "true") {
        boxAlerta.className = "critico";
        boxAlerta.innerHTML = "⚠️ QUALIDADE RUIM";
    } else {
        boxAlerta.className = "normal";
        boxAlerta.innerHTML = "✅ QUALIDADE BOA";
    }
    
    document.getElementById('txtVazao').innerText = data.tem_vazao ? "ESTADO: COM VAZÃO" : "ESTADO: SEM VAZÃO";
});