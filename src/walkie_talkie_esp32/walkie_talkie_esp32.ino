#include <esp_now.h>
#include <WiFi.h>

// Pinos e Configurações
#define AUDIO_IN_PIN 34       // Entrada de áudio (ADC)
#define AUDIO_OUT_PIN 25      // Saída de áudio (DAC)
#define PTT_BUTTON_PIN 13     // Botão Push-to-Talk
#define SAMPLE_RATE 8000      // Taxa de amostragem (8 kHz)
#define BUFFER_SIZE 64        // Tamanho do buffer de áudio

// Estados
bool isTransmitting = false;  // Indica se está em modo TX
bool isReceiving = false;     // Indica se está em modo RX

// Buffers
uint8_t audioBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Endereço MAC do outro ESP32 (substitua com o MAC real)
uint8_t peerMAC[] = {0x24, 0x6F, 0x28, 0xXX, 0xXX, 0xXX};

// Timer
hw_timer_t *timer = NULL;

// Função de Captura de Áudio
void IRAM_ATTR captureAudio() {
    if (bufferIndex < BUFFER_SIZE) {
        int adcValue = analogRead(AUDIO_IN_PIN);    // Lê o áudio do microfone
        audioBuffer[bufferIndex++] = adcValue >> 4; // Reduz de 12 bits para 8 bits
    }
}

// Função para Enviar Áudio
void sendAudio() {
    if (bufferIndex == BUFFER_SIZE) {
        esp_now_send(peerMAC, audioBuffer, BUFFER_SIZE); // Envia o buffer
        bufferIndex = 0; // Reseta o índice
    }
}

// Função de Recebimento de Áudio
void onReceive(const uint8_t *macAddr, const uint8_t *data, int len) {
    if (len == BUFFER_SIZE) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            dacWrite(AUDIO_OUT_PIN, data[i] << 4); // Reproduz o áudio recebido
        }
    } else if (len == 1) { // Mensagem de controle
        isReceiving = data[0] == 1; // 1: Outro ESP está transmitindo
        isTransmitting = !isReceiving;
    }
}

void setup() {
    // Configurações de Rede
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_recv_cb(onReceive);

    // Configuração do Peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, peerMAC, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    // Configuração de Pinos
    pinMode(PTT_BUTTON_PIN, INPUT_PULLUP);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    pinMode(AUDIO_OUT_PIN, OUTPUT);

    // Timer para Amostragem de Áudio
    timer = timerBegin(0, 80, true); // Timer a 80 MHz
    timerAttachInterrupt(timer, &captureAudio, true);
    timerAlarmWrite(timer, 1000000 / SAMPLE_RATE, true);
    timerAlarmEnable(timer);
}

void loop() {
    // Verifica o estado do botão PTT
    bool pttPressed = digitalRead(PTT_BUTTON_PIN) == LOW;

    if (pttPressed && !isTransmitting) {
        // Entra em modo transmissão
        uint8_t controlMessage = 1;
        esp_now_send(peerMAC, &controlMessage, 1);
        isTransmitting = true;
        isReceiving = false;
    } else if (!pttPressed && !isReceiving) {
        // Entra em modo recepção
        uint8_t controlMessage = 0;
        esp_now_send(peerMAC, &controlMessage, 1);
        isTransmitting = false;
        isReceiving = true;
    }

    // Transmissão de áudio
    if (isTransmitting) {
        sendAudio();
    }
}
