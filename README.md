# ESP32 Walkie-Talkie Bidirecional

Este projeto implementa um walkie-talkie bidirecional utilizando dois módulos ESP32 e o protocolo **ESP-NOW** para comunicação off-grid. Cada módulo pode alternar entre os papéis de transmissor e receptor (half-duplex), permitindo comunicação bidirecional eficiente.

---

## **Características do Projeto**
- **Protocolo ESP-NOW**: Comunicação rápida e sem a necessidade de conexão Wi-Fi.
- **Half-Duplex**: Alternância entre transmissão e recepção com base no estado do botão Push-to-Talk (PTT).
- **Áudio Dinâmico**:
  - Captura de áudio via ADC (Entrada Analógica do ESP32).
  - Reprodução de áudio via DAC (Saída Analógica do ESP32).
- **Mensagens de Controle**: Gerenciamento do estado de transmissão e recepção entre os módulos.

---

## **Hardware Necessário**

1. **ESP32**: 2 unidades.
2. **Microfone Electret com Amplificador** (exemplo: MAX9814).
3. **Alto-falante**: 1W, 8Ω.
4. **Amplificador de Áudio**: PAM8403 ou equivalente.
5. **Botão Push-to-Talk (PTT)**: Um botão para cada ESP32.
6. **Resistores de Pull-Up**: Para o botão (se necessário).
7. **Fonte de Alimentação**: Bateria LiPo ou fonte de 5V.

---

## **Esquema de Conexão**

| Componente         | ESP32         |
|--------------------|---------------|
| Microfone (OUT)    | ADC (Pino 34) |
| Alto-falante       | DAC (Pino 25) |
| Botão PTT          | GPIO 13       |

- **Alimentação**: Certifique-se de que todos os componentes estejam alimentados corretamente (3.3V ou 5V, conforme o módulo utilizado).

---

## **Instalação e Configuração**

1. **Clone o repositório do git**
   ```bash
   git clone ...
   ```

2. **Configure o Ambiente de Desenvolvimento:**
   - Instale a [Arduino IDE](https://www.arduino.cc/en/software) ou [PlatformIO](https://platformio.org/).
   - Certifique-se de que as bibliotecas necessárias estejam instaladas:
     - `esp_now`
     - `WiFi`

3. **Ajuste os Endereços MAC:**
   - Localize os endereços MAC dos ESP32:
     ```cpp
     WiFi.macAddress();
     ```
   - Substitua os endereços no código pelo endereço MAC do outro dispositivo:
     ```cpp
     uint8_t peerMAC[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
     ```

4. **Carregue o Código:**
   - Conecte o ESP32 ao computador.
   - Faça o upload do código disponível no arquivo `esp32_walkie_talkie.ino`.

---

## **Funcionamento**

1. **Botão Push-to-Talk:**
   - **Pressionado**: O ESP32 entra no modo de transmissão.
   - **Solto**: O ESP32 entra no modo de recepção.

2. **Mensagens de Controle:**
   - Quando um módulo está transmitindo, o outro automaticamente entra no modo de recepção.

3. **Áudio:**
   - O áudio capturado pelo microfone é enviado ao outro módulo via ESP-NOW.
   - O módulo receptor reproduz o áudio no alto-falante.

---

## **Parâmetros do Código**

- **Taxa de Amostragem:** 8 kHz
- **Tamanho do Buffer de Áudio:** 64 bytes
- **Resolução do ADC:** 12 bits

---

## **Melhorias Futuras**
- **Compressão de Áudio:** Implementar ADPCM para reduzir o tamanho dos dados transmitidos.
- **Filtro Digital:** Adicionar filtragem para suavizar o áudio capturado.
- **Indicação Visual:** LEDs para indicar o estado (TX/RX).
- **Alcance:** Uso de antenas externas para aumentar o alcance.

---

## **Licença**
Este projeto é distribuído sob a licença MIT. Sinta-se livre para usá-lo e modificá-lo conforme necessário.

---

## **Contribuições**
Contribuições são bem-vindas! Envie pull requests ou abra issues para reportar problemas e sugerir melhorias.
