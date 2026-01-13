"use strict"

document.addEventListener('DOMContentLoaded', () => {
    fetchDataAndDisplay();
    setInterval(fetchDataAndDisplay, 20000);
});

async function fetchDataAndDisplay() {
    const url = "https://api.allorigins.win/raw?url=https://room.heitorlima.tech/environment/lastmetric";
    
    try {
        const response = await fetch(url);
        if (!response.ok) throw new Error('Error');
        
        const data = await response.json();
        
        document.querySelector('.temp').textContent = `Temperature: ${data.temperature}°C`;
        document.querySelector('.hum').textContent = `Humidity: ${data.humidity}%`;
        document.querySelector('.heatindex').textContent = `Heat Index: ${data.heatindex}`;
        
    } catch (error) {
        document.getElementById('data').innerHTML = `<p style="color: red;">Error}</p>`;
    }
}