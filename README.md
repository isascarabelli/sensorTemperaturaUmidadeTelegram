# Sensor de temperatura e umidade usando o telegram
Firmware para monitoramento e notificação remota, via Telegram, de umidade e temperatura de laboratórios de pesquisa do ICB/UFMG.


## Descrição do Programa de Computador:

Programa desenvolvido para o sistema embarcado ESP32 com a finalidade de coletar dados relacionados à umidade e temperatura de laboratório do ICB da UFMG, informando, via rede sem fio, aos usuários pré conectados ao bot do telegram do projeto (porfessores, diretores, responsáveis pelo laboratório, etc), caso a umidade esteja abaixo de 50%, e 30% nos casos mais graves. Essa comunicação entre usuário e sistema se dá por meio de um bot criado pelo serviço de mensagens instantâneas, Telegram, o qual é possível compartilhar com qualquer pessoa que possui o serviço, seja em dispositivos móveis ou não.. As medidas de umidade e temperatura também são apresentadas em um display LCD para os pesquisadores que estão presentes no laboratório.

