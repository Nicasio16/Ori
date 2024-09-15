require('dotenv').config();
const WebSocket = require('ws');
const axios = require('axios');
const fs = require('fs'); // Import the fs module

// Function to generate a developer token using the dev ID and API key
async function generateDevToken(devId, apiKey) {
    try {
        const response = await axios.post('https://ws.tryterra.co/auth/developer', {}, {
            headers: {
                'accept': 'application/json',
                'dev-id': devId,
                'x-api-key': apiKey,
            }
        });
        
        return response.data.token || '';
    } catch (error) {
        console.error(`Failed to generate dev token: ${error}`);
        return null;
    }
}

// Function to send a heartbeat message to the WebSocket server every 40 seconds
async function sendHeartbeat(websocket) {
    while (websocket.readyState === WebSocket.OPEN) {
        websocket.send(JSON.stringify({ op: 0 }));
        await new Promise(resolve => setTimeout(resolve, 40000)); // Wait 40 seconds before sending the next heartbeat
    }
}

// Main function to manage WebSocket connection and message handling
async function main() {
    const uri = "wss://ws.tryterra.co/connect";
    const devId = "4actk-healthapp-testing-80xqarUDhD";
    const apiKey = "rHjOsuy9Q0ZGfkfeNE05jMvDAx8lTvDK";

    // Ensure the necessary environment variables are available
    if (!devId || !apiKey) {
        console.error("Missing environment variables DEV_ID or API_KEY");
        return;
    }

    // Generate an authentication token
    const authToken = await generateDevToken(devId, apiKey);
    if (!authToken) {
        console.error("Failed to retrieve auth token");
        return;
    }

    // Prepare the authentication payload
    const authPayload = {
        op: 3,
        d: {
            token: authToken,
            type: 1,
        },
    };

    // Connect to the WebSocket server
    const websocket = new WebSocket(uri);

    websocket.on('open', async () => {
        // Send the authentication payload to authenticate the connection
        websocket.send(JSON.stringify(authPayload));
        
        // Start the heartbeat function to keep the connection alive
        sendHeartbeat(websocket);
    });

    // Handle incoming messages
    websocket.on('message', (message) => {
        const msg = JSON.parse(message);
        console.log(msg); // Parse the message and print it

        // Check if the message type is HEART_RATE and extract the value
        if (msg.t === 'HEART_RATE' && msg.d && msg.d.val !== undefined) {
            const heartRate = msg.d.val;
            const logMessage = `Heart Rate: ${heartRate}\n`;

            // Append the heart rate value to a text file
            fs.appendFile('heart_rate_log.txt', logMessage, (err) => {
                if (err) {
                    console.error(`Failed to write to file: ${err}`);
                }
            });
        }
    });

    // Handle connection errors
    websocket.on('error', (error) => {
        console.error(`WebSocket error: ${error}`);
    });

    // Handle connection close
    websocket.on('close', () => {
        console.log('WebSocket connection closed');
    });
}

// Entry point to run the main function
main();