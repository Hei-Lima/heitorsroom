"use strict"

document.addEventListener('DOMContentLoaded', () => {
    fetchDataAndDisplay();
    setInterval(fetchDataAndDisplay, 20000);
});

async function fetchDataAndDisplay() {
    const url = "https://room.heitorlima.tech/environment/lastmetric";
    
    try {
        const response = await fetch(url);
        if (!response.ok) throw new Error('Error');
        
        const data = await response.json();
        
        document.querySelector('.temp').textContent = `${data.temperature}°C`;
        document.querySelector('.hum').textContent = `${data.humidity}%`;
        document.querySelector('.heatindex').textContent = `${data.heatindex}`;
        
    } catch (error) {
        document.getElementById('data').innerHTML = `<p style="color: red;">Error}</p>`;
    }
}