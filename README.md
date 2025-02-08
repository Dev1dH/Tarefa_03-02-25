# Tarefa_03-02-25
Vídeo da demonstração: https://youtu.be/WGAawecnusw
## Objetivos
- Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores
- Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.
- Manipular e controlar LEDs comuns e LEDs endereçaveis WS2812
- Fixar o estudo do uso de botões de acionamento, interrupção e debounce.
- Desenvolver um projeto funcional que combine hardware e software.

## Componentes do projeto
- Matriz 5x5 de LEDs (endereçaveis) WS2812, conectada a GPIO 7.
- LED RGB, com os pinos conectados as GPIOS (11, 12 e 13).
- Botão A conectado a GPIO 5
- Botão B conectado a GPIO 6
- Display SSD1306 conectado via I2C (GPIO14 e GPIO15)

### Funcionamento do projeto

### 1. Interação com o botão A
- Ao pressionar o botão A o estado do LED RGB verde se altera (ligado/desligado).
- Uma mensagem é mostrada no display SSD1306 e no Serial Monitor "LED VERDE ON" e "LED VERDE OFF".
### 2. Interação com o botão B
- Ao pressionar o botão B o estado do LED RGB azul se altera (ligado/desligado)
- Uma mensagem é mostrada no display SSD1306 e no Serial Monitor "LED AZUL ON" e "LED AZUL OFF".
### 3. Interação com serial monitor
- Digitar qualquer caracterer entre A e Z e 0 e 9, reproduz o mesmo caracter no display OLED.
- Se o caracter for entre um dígito, o dígito correspondente é desenhado na matriz de LEDs.
