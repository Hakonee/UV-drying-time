# UV Drying Time - Descrição Detalhada do Projeto

## Visão Geral

O UV Drying Time é um sistema eletrônico baseado em Arduino projetado para gerenciar o tempo de exposição à luz ultravioleta (UV) em processos de cura, como a secagem de resinas UV em impressão 3D, esmaltes UV em manicure ou outros materiais sensíveis ao tempo de exposição. O dispositivo oferece uma interface intuitiva para configurar e monitorar a duração da cura, com feedback visual (via display LCD) e sonoro (via buzzer), além de automação para acionar a luz UV por meio de um relé. Ele é voltado para hobbistas, makers, profissionais de pequenas indústrias ou educadores, proporcionando precisão, praticidade e flexibilidade em processos de cura UV.

O sistema utiliza um cronômetro regressivo configurável, permitindo que o usuário defina tempos de cura entre 30 segundos e 30 minutos, com controles para iniciar, pausar, ajustar e reiniciar o processo. O projeto combina hardware acessível com software otimizado, tornando-o ideal para aplicações práticas e aprendizado de eletrônica e programação.

## Objetivo
O objetivo principal do UV Drying Time é automatizar e simplificar o controle do tempo de exposição à luz UV, eliminando a necessidade de temporização manual e garantindo que o material receba a dose exata de UV necessária para a cura. O sistema visa:

- Proporcionar uma interface clara para configuração e acompanhamento do tempo.
- Oferecer feedback imediato sobre o progresso e conclusão do processo.
- Automatizar o acionamento da luz UV, aumentando a eficiência.
- Ser acessível, usando componentes comuns e código aberto.


## Componentes
O projeto utiliza componentes eletrônicos amplamente disponíveis, permitindo fácil replicação. Abaixo está a lista completa com suas funções:

1. #### Arduino Uno (ou similar, como Nano):

- Microcontrolador central**, responsável por executar o firmware, processar entradas dos botões, gerenciar o display, controlar o cronômetro e acionar o relé e o buzzer.
- Alternativas como o Arduino Nano** podem ser usadas para reduzir o tamanho do projeto.


2. #### Display LCD 16x2 com Módulo I2C:

- Exibe informações em duas linhas de 16 caracteres, como o tempo configurado (ex.: "Set: 05:00"), o tempo restante (ex.: "Rest: 04:30") e mensagens de status (ex.: "Cura Concluída!").
- O módulo I2C reduz o número de pinos necessários, usando apenas SDA (A4) e SCL (A5) para comunicação.


3. #### Botões (3 unidades):

- Botão Ajuste (pino 4): Incrementa o tempo de cura em passos de 30 segundos, permitindo configurar o cronômetro antes de iniciar.
- Botão Iniciar/Pausar ( (pino 2): Inicia a contagem regressiva ou pausa/retoma o processo durante a operação.
- Botão Reset ( (pino 3): Reinicia o cronômetro para o tempo padrão (5 minutos) e desliga a luz UV.
- Configurados com pull-up interno para simplificar o circuito.


4. #### Buzzer:

- Conectado ao pino 5, emite sons para feedback:
  - 1000 Hz (início ou retomada).
  - 800 Hz (pausa).
  - 600 Hz (reset).
  - 1200 Hz (conclusão, som longo).


- Ajuda a sinalizar ações mesmo sem olhar para o display.


5. #### Relé:

- Conectado ao pino 6, controla a luz UV, ligando-a durante a contagem e desligando-a ao pausar ou concluir.
- Compatível com luzes UV de baixa ou alta potência, desde que respeite as especificações do relé.


6. #### Outros:

- Fonte de alimentação: 5V para o Arduino (via USB ou adaptador) e, se necessário, fonte separada para a luz UV.
- Cabos e protoboard: Para conexões temporárias ou placa de circuito para montagem permanente.




## Funcionalidades
O UV Drying Time oferece as seguintes funcionalidades principais, projetadas para facilitar o controle de processos de cura UV:

1. Configuração de Tempo:

  - O usuário ajusta o tempo de cura (padrão: 5 minutos) usando o botão Ajuste, que incrementa o valor em intervalos de 30 segundos, até um máximo de 30 minutos.
  - O display LCD exibe o tempo configurado em tempo real no formato MM:SS (ex.: "Set: 07:30").


2. Contagem Regressiva:

  - Após pressionar o botão Iniciar/Pausar, o cronômetro inicia a contagem regressiva, atualizando o display a cada segundo com o tempo restante (ex.: "Rest: 04:59").
  - O relé liga a luz UV automaticamente no início da contagem.


3. Pausa e Retomada:

  - Durante a contagem, o botão Iniciar/Pausar permite pausar o cronômetro, desligando a luz UV e mantendo o tempo restante. Pressionar novamente retoma a contagem do ponto pausado.


4. Reinício:

  - O botão Reset interrompe qualquer contagem, desliga a luz UV e retorna o cronômetro ao tempo padrão de 5 minutos, exibindo "Set: 05:00".


5. Feedback Visual e Sonoro:

  - Display LCD: Mostra mensagens claras, como "UV Drying Time" na inicialização, "Cura em Andamento" durante a contagem e "Cura Concluída!" ao final.
  - Buzzer: Emite tons distintos para cada ação, permitindo acompanhamento auditivo.
  - Relé: Garante que a luz UV só esteja ativa quando necessário, com feedback implícito pelo acionamento.


6. #### Robustez:

  - O código usa millis() para um cronômetro não bloqueante, evitando atrasos ou interrupções.
  - Botões possuem debounce (atraso de 300 ms) para evitar múltiplas leituras acidentais.




## Fluxo de Operação
O sistema segue um fluxo lógico e intuitivo:

1. Inicialização:

  - Ao ligar o Arduino, o LCD exibe "UV Drying Time" por 2 segundos, seguido do tempo padrão de 5 minutos ("Set: 05:00").
  - O relé permanece desligado, e o sistema aguarda interação.


2. Configuração:

  - O usuário pressiona o botão Ajuste para aumentar o tempo em incrementos de 30 segundos. O LCD atualiza o valor em tempo real.
  - Exemplo: Pressionar duas vezes a partir de 05:00 resulta em "Set: 06:00".


3. Início da Contagem:

  - Pressionar o botão Iniciar/Pausar inicia a contagem regressiva, liga o relé (ativando a luz UV) e exibe "Cura em Andamento" com o tempo restante (ex.: "Rest: 04:59").
  - O buzzer emite um som de 1000 Hz por 200 ms.


4. Pausa/Retomada:

  - Durante a contagem, pressionar Iniciar/Pausar pausa o cronômetro, desliga o relé e emite um som de 800 Hz. O LCD mantém o tempo restante.
  - Pressionar novamente retoma a contagem, religa o relé e emite um som de 1000 Hz.


5. Conclusão:

  - Ao atingir zero, o relé desliga a luz UV, o LCD exibe "Cura Concluída!" e o buzzer toca um som de 1200 Hz por 1 segundo.
  - Após 2 segundos, o sistema retorna ao estado inicial ("Set: 05:00").


6. Reset:

  - A qualquer momento, pressionar o botão Reset interrompe a contagem, desliga o relé, emite um som de 600 Hz e reinicia o tempo para 5 minutos.




## Aplicações
O UV Drying Time é versátil e pode ser usado em diversos cenários que requerem controle preciso de exposição UV:

  - Impressão 3D: Cura de resinas UV em peças impressas, garantindo a solidificação adequada.
  - Manicure: Temporização de secagem de esmaltes UV ou géis em unhas.
  - Artesanato: Criação de joias ou objetos com resinas UV.
  - Indústria Leve: Processos de cura em pequenas produções, como adesivos ou revestimentos UV.
  - Educação: Projeto prático para ensinar eletrônica, programação Arduino e conceitos de temporização.


## Detalhes Técnicos
### Hardware

- ### Conexões:


| Componente   | Pino Arduino | Função |
|:----------   |:-------------|:-------|
|LCD SDA       |       A4     | Comunicação I2C (dados) |
|LCD SCL       |       A5     | Comunicação I2C (clock)|
|Botão Iniciar |       2      | Iniciar/pausar contagem |
|Botão Reset   |       3      | Reiniciar cronômetro |
|Botão Ajuste  |       4      | Ajustar tempo |
|Buzzer        |       5      | Feedback sonoro |
|Relé          |       6      | Controle da luz UV |



- #### Alimentação:
  Arduino alimentado por USB (5V) ou adaptador. A luz UV pode exigir fonte separada, dependendo do relé.
- #### Endereço I2C do LCD:
  Comum em 0x27 ou 0x3F. Deve ser verificado com um scanner I2C se o display mostrar caracteres estranhos.

### Software

- Código: Escrito em C++ para Arduino, disponível no arquivo UV_Drying_Time.ino. Principais características:
  - Usa millis() para temporização não bloqueante, permitindo multitarefa.
  - Implementa debounce nos botões (atraso de 300 ms).
  - Gerencia estados (configuração, contagem, pausa, concluído) com variáveis booleanas.


### Bibliotecas:
  - Wire.h: Para comunicação I2C.
  - LiquidCrystal_I2C.h (Frank de Brabander): Para controle do display LCD.


### Estrutura do Código:
  - setup(): Inicializa o LCD, configura pinos e exibe a mensagem de boas-vindas.
  - loop(): Monitora botões, atualiza o cronômetro e gerencia o relé/buzzer.
  - Funções auxiliares: mostrarTempo() e mostrarTempoRestante() formatam a exibição do tempo no LCD.



### Configuração

- Instalação:
  1. Instale o Arduino IDE.
  2. Adicione a biblioteca LiquidCrystal_I2C via Sketch > Include Library > Manage Libraries.
  3. Conecte os componentes conforme a tabela de conexões.
  4. Carregue o código UV_Drying_Time.ino no Arduino.
 

## Imagens:
![image](https://github.com/user-attachments/assets/f122df6c-f421-4cda-b367-535ae0e1a50c)
![IMG_20250708_135753](https://github.com/user-attachments/assets/2d521171-4d47-4d45-a348-a7736e8b8582)
![IMG_20250708_135741](https://github.com/user-attachments/assets/3347c22f-d44b-4da0-8b81-f70899a3f0aa)
![Screenshot_2025-07-08-13-55-15-914_com instagram android](https://github.com/user-attachments/assets/e9e521d2-0eed-4567-bab2-a0a02095d223)

