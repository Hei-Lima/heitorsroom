"use strict"

document.addEventListener('DOMContentLoaded', () => {
    fetchDataAndDisplay();
    setInterval(fetchDataAndDisplay, 20000);
});

async function fetchDataAndDisplay() {
    const API_BASE = 'https://room.heitorlima.tech';
    const url = `${API_BASE}/environment/lastmetric`;
    
    try {
        const response = await fetch(url);
        if (!response.ok) throw new Error('Error');
        
        const data = await response.json();

        // update only the value spans so existing label/value classes stay in place
        const tempVal = document.querySelector('.temp .metric-value');
        const humVal = document.querySelector('.hum .metric-value');
        const heatVal = document.querySelector('.heatindex .metric-value');

        if (tempVal) tempVal.textContent = `${data.temperature}°C`;
        if (humVal) humVal.textContent = `${data.humidity}%`;
        if (heatVal) heatVal.textContent = `${data.heatindex}`;

        // update timestamp if provided, else use now
        const tsEl = document.getElementById('timestamp');
        if (tsEl) {
            const ts = data.timestamp ? new Date(data.timestamp) : new Date();
            tsEl.textContent = ts.toLocaleString();
        }
        
    } catch (error) {
        document.getElementById('data').innerHTML = `<p style="color: red;">Error}</p>`;
    }
}