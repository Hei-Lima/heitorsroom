const form = document.getElementById('messageForm');
const responseP = document.getElementById('response');
const charCount = document.getElementById('charCount');

// Update remaining characters on input
const messageEl = document.getElementById('message');
if (messageEl && charCount) {
    const updateCount = () => {
        const remaining = 80 - (messageEl.value?.length || 0);
        charCount.textContent = String(remaining);
    };
    messageEl.addEventListener('input', updateCount);
    updateCount();
}

if (form) {
    form.addEventListener('submit', async (event) => {
        event.preventDefault();

        const message = document.getElementById('message').value;
        const username = document.getElementById('username') ? document.getElementById('username').value : 'anonymous';

        if (message.length > 80) {
            if (responseP) responseP.textContent = 'Message must be 80 characters or less.';
            return;
        }

        try {
            const API_BASE = 'https://room.heitorlima.tech';
            const response = await fetch(`${API_BASE}/message`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ message, username }),
            });

            if (response.ok) {
                const successText = "heitor was contacted. he's alerted of your existence";
                
                const boxSection = document.querySelector('.box.data-section');
                if (boxSection) {
                    boxSection.innerHTML = `<div class="msg-box"><p>${successText}</p></div>`;
                }
            } else {
                if (responseP) responseP.textContent = 'connection with heitor was unsuccessful. is he away?';
            }
        } catch (error) {
            console.error('Error:', error);
            if (responseP) responseP.textContent = 'An error occurred while sending the message.';
        }
    });
}