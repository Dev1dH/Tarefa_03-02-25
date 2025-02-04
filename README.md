# Tarefa_03-02-25

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

## Funcionamento do projeto

# 1. Interação com o botão A
- Ao pressionar o botão A o estado do LED RGB verde se altera (ligado/desligado).
- A operação deve ser registrada tanto no display SSD1306 quanto no Serial Monitor.
# 2. Interação com o botão B
- Ao pressionar o botão B o estado do LED RGB azul se altera (ligado/desligado)
- A operaração deve ser registrada tanto no display SSD1306 quanto no Serial Monitor
- 
