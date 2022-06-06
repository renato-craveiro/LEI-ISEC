;---------------------------------------------------------------------
;|TRABALHO REALIZADO POR RENATO CRAVEIRO | 2018011392                |
;|INTRODUÇÃO À INTELIGÊNCIA ARTIFICIAL | LIC. ENGENHARIA INFORMÁTICA |
;|INSTITUTO SUPERIOR DE ENGENHARIA DE COIMBRA | ANO-LETIVO 2021/2022 |
;---------------------------------------------------------------------


;O objetivo deste trabalho consiste em conceber, implementar e analisar comportamentos
;racionais para agentes reativos. O trabalho deve ser realizado na ferramenta Netlogo.

;--------------------------------------------------------------------

;No ambiente existem dois tipos de agentes: os Comilões e os Limpadores
breed [comiloes comilao]
breed [limpadores limpador]

turtles-own [energia]
;São agentes reativos com memória.
limpadores-own[deposito_lixo]


breed [zombies zombie]

to setup

  clear-all
  setup-patches
  setup-turtles
  reset-ticks

end

to Go
  MoveComiloes
  MoveLimpadores
  MoveZombies

  if count turtles = 0 or ticks = 25000 or (zombie? and (count comiloes = 0 and count limpadores = 0)) or (reproducao? and count comiloes >= 10000)
  [
    stop
  ]

  tick
end

to setup-patches
  ask patches
    [
    ;2 tipos de resíduos -
    ;lixo normal (células amarelas) e lixo tóxico (células vermelhas) -
    ;e um tipo de alimento (células verdes).

    ;A percentagem de células do ambiente que contêm resíduos ou alimento é configurável:
    ;entre 0% e 15% para cada tipo de resíduo e
        if random 101 < lixo_normal
        [
            set pcolor yellow
        ]

        if random 101 < lixo_toxico
        [

            set pcolor red
        ]

    ;5% - 20% para alimento.
    ;A energia obtida pela ingestão do alimento também é configurável, variando
    ;entre 1 e 50.
        if random 101 < qtd_alimento
        [
            set pcolor green
        ]


    ]

  ;número configurável de depósitos (células azuis) onde
  ;alguns agentes podem depositar o lixo.
  ;A quantidade de depósitos varia entre 1 e 10.

    repeat n_depositos
        [

            ask one-of patches with [pcolor = black]
            [
                set pcolor blue
            ]
        ]

end


;O alimento e os resíduos devem reaparecer no mundo de tal forma que os níveis
;configurados se mantenham ao longo da simulação.

to regenera_alimento
  ask one-of patches with [pcolor = black]
  [
    set pcolor green
  ]
end

to regenera_lixoT
  ask one-of patches with [pcolor = black]
  [
    set pcolor red
  ]
end

to regenera_lixoN
  ask one-of patches with [pcolor = black]
  [
    set pcolor yellow
  ]
end



to setup-turtles
  create-comiloes ncomiloes
  create-limpadores nlimpadores
  ask comiloes
  [
    set energia 100
    set shape "person"
    set color white
    setxy random-xcor random-ycor;
    while [[pcolor] of patch-here != black]
    [
      setxy random-xcor random-ycor;
    ]

  ]
  ask limpadores
  [
    set energia 100
    set shape "triangle"
    set color orange
    setxy random-xcor random-ycor;
    while [[pcolor] of patch-here != black]
    [
      setxy random-xcor random-ycor;
    ]

  ]

end

to MoveZombies
  ask zombies
  [
    let mortais comiloes in-radius 1
    ifelse any? mortais
    [ face one-of mortais ]
    [ rt (random 4) * 90 ]
    fd 1
  ]
end

to MoveComiloes
  ask comiloes
  [
    if (reproducao?) and (energia >= 150)
    [
      hatch 1 [ set energia energia * (1 / 2)
        lt 90
        fd 1 ]
      set energia energia * (1 / 4)
    ]

    if (zombie?) and (energia <= 0)
    [

      hatch-zombies 1
      [
        set shape "person"
        set color 52
        lt 90 fd 1
      ]
      die
    ]



    if (zombie?)
    [
      if count zombies-here >= 1
      [
        die
        hatch-zombies 1
        [
          set shape "person"
          set color 52
          lt 90 fd 1
        ]

      ]
    ]




    ;os  Comilões  conseguem  percecionar  o  conteúdo  das  células  que  se
    ;encontram imediatamente à sua frente, à sua esquerda e à sua direita.
    ;           patches at-points [[1 0] [0 1] [-1 0]]

    ;os  Comilões  podem  deslocar-se  para  a  célula  imediatamente  à  sua frente,
    ;rodar  90º  para  a  esquerda  ou  rodar  90º  para  a  direita.
    ;                lt 90                              rt 90

    ;São agentes puramente reativos



    ;Se  os  Comilões  se  posicionarem  numa  célula  contendo  algum  tipo  de
    ;resíduo,  morrem.  As  perceções/ações  dos  Comilões  devem  tentar  evitar
    ;estas situações.
    if ([pcolor] of patch-here = red) or ([pcolor] of patch-here = yellow)
    [
      if(zombie?)
      [
        hatch-zombies 1
        [
          set shape "person"
          set color 52
          lt 90 fd 1
        ]
      ]
      die
    ]


    ;Os Comilões ingerem automaticamente os alimentos que se encontrem na
    ;célula atual. Se isto suceder, a célula passa a ter a cor preta e a energia do
    ;agente aumenta de acordo com a configuração do ambiente.
    if [pcolor] of patch-here = green
    [
      set energia (energia + alim_energia)
      set pcolor black
      regenera_alimento
    ]

    ;Se os Comilões percecionarem uma célula contendo algum tipo de
    ;resíduo, a sua energia deve ser reduzida da seguinte forma:


    ;Decrementar 5% da sua energia atual se percecionaram lixo
    ;normal
    ifelse [pcolor] of patches at-points [[1 0] [0 1] [-1 0]] = yellow
    [
      set energia  (energia - (energia * 0.05))
      ifelse [pcolor] of patch-at-heading-and-distance 90 1 = yellow
      [
        lt 90
        set energia energia - 1
      ]
      [
        ifelse [pcolor] of patch-at-heading-and-distance -90 1 = yellow
        [
          rt 90
          set energia energia - 1
        ]
        [
          ifelse random 101 < 50
          [
            rt 90
            set energia energia - 1
          ]
          [
            lt 90
            set energia energia - 1
          ]
        ]
      ]
    ]
    [
      ;Decrementar 10% da sua energia atual se percecionaram lixo
      ;tóxico
      ifelse [pcolor] of patches at-points [[1 0] [0 1] [-1 0]] = red
      [
        set energia  (energia - (energia * 0.1))
        ifelse [pcolor] of patch-at-heading-and-distance 90 1 = red
          [
            lt 90
            set energia energia - 1
        ]
        [
          ifelse [pcolor] of patch-at-heading-and-distance -90 1 = red
          [
            rt 90
            set energia energia - 1
          ]
          [
            ifelse random 101 < 50
            [
              rt 90
              set energia energia - 1
            ]
            [
              lt 90
              set energia energia - 1
            ]
          ]
        ]
      ]
      [
        ifelse [pcolor] of patches at-points [[1 0] [0 1] [-1 0]] = green
        [
          ifelse [pcolor] of patch-at-heading-and-distance 90 1 = green
          [
            rt 90
            set energia energia - 1
          ]
          [
            ifelse [pcolor] of patch-at-heading-and-distance -90 1 = green
            [
              lt 90
              set energia energia - 1
            ]
            [
              fd 1
              set energia energia - 1
            ]
          ]
        ]
       [
          ifelse random 101 < 50
          [
            fd 1
            set energia energia - 1
          ]
          [
            ifelse random 101 < 50
            [
              rt 90
              set energia energia - 1
            ]
            [
              lt 90
              set energia energia - 1
            ]
          ]
        ]
      ]
    ]
  ]
end

to MoveLimpadores
  ask limpadores
  [
    if energia = 0
    [
      die
    ]

    if (zombie?)
    [
      if count zombies-here >= 1
      [
        die
        hatch-zombies 1
        [
          set shape "person"
          set color 52
          lt 90 fd 1
        ]

      ]
    ]

    ifelse [pcolor] of patch-here = green
      [
        ;Mesmo  com  o  depósito  cheio,  os  Limpadores  podes  continuar  a  comer
        ;alimento,  recebendo  menos  energia  do  alimento  (ver  abaixo  a  regra  de
        ;aumento de energia).
        ;Se  o  número  de  resíduos  que  transporta  for  menos  de  metade  do
        ;limite,  o  aumento  de  energia  corresponde  ao  valor  indicado  na
        ;configuração do ambiente.
        ifelse(deposito_lixo > 0.5 * limite_lixo)
        [
          set energia (energia + (alim_energia * 0.5))
          set pcolor black
          regenera_alimento
        ]
        [
          ;Caso  contrário,  o  aumento  de  energia  corresponde  a  metade  do
          ;valor indicado na configuração.
          set energia (energia + (alim_energia))
          set pcolor black
          regenera_alimento
        ]
    ]
    [
      ;Ao  encontrar  o  depósito,  a  variável  volta  a  zero,  o  acumulado  no
      ;depósito é atualizado e o nível de energia do Limpador é
      ;aumentado de 10*número de células depositadas.
      if [pcolor] of patch-here = blue
      [
        set energia energia + (10 * deposito_lixo)
        set deposito_lixo 0
      ]
    ]


    ;conseguem  percecionar  o  conteúdo  das  células  que  se  encontram
    ;imediatamente à sua frente e à direita.

    ;Podem transportar uma quantidade limitada de resíduos (configurável). Ao
    ;atingirem  esse  limite,  os  Limpadores  têm  de  encontrar  um  dos  depósitos
    ;(células azuis) e despejar os resíduos, antes de continuar a recolha.
    ifelse (deposito_lixo >= limite_lixo)
    [
      ifelse [pcolor] of patch-ahead 1 = blue
      [
        fd 1
        set energia energia - 1
      ]
      [
        ifelse [pcolor] of patch-at-heading-and-distance 90 1 = blue
        [
          rt 90
          set energia energia - 1
        ]
        [
          ifelse random 101 < 50
          [
            fd 1
            set energia energia - 1
          ]
          [
            ifelse random 101 < 50
            [
              rt 90
              set energia energia - 1
            ]
            [
              lt 90
              set energia energia - 1
            ]
          ]
        ]
      ]
    ]
    [
      ;Ao  recolher  um  resíduo  tóxico  deve  ser  incrementada  de  duas
      ;unidades;
      ifelse [pcolor] of patch-here = red
      [
        set deposito_lixo deposito_lixo + 2
        set pcolor black
        regenera_lixoT
      ]
      [
        ;Ao  recolher  um  resíduo  normal  deve  ser  incrementada  de  uma
        ;unidade
        if [pcolor] of patch-here = yellow
        [
          set deposito_lixo deposito_lixo + 1
          set pcolor black
          regenera_lixoN
        ]
      ]

      ifelse ([pcolor] of patch-ahead 1 = red) or ([pcolor] of patch-ahead 1 = yellow)
      [
        fd 1
        set energia energia - 1
      ]
      [
        ifelse ([pcolor] of patch-at-heading-and-distance 90 1 = red) or ([pcolor] of patch-at-heading-and-distance 90 1 = yellow)
        [
          rt 90
          set energia energia - 1
        ]
        [
          ifelse random 101 < 50
          [
            fd 1
            set energia energia - 1
          ]
          [
            ifelse random 101 < 50
            [
              rt 90
              set energia energia - 1
            ]
            [
              lt 90
              set energia energia - 1
            ]
          ]
        ]
      ]

    ]
  ]

end

@#$#@#$#@
GRAPHICS-WINDOW
555
43
992
481
-1
-1
13.0
1
10
1
1
1
0
1
1
1
-16
16
-16
16
0
0
1
ticks
30.0

BUTTON
590
506
653
539
NIL
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
504
401
537
551
lixo_normal
lixo_normal
0
15
15.0
1
1
NIL
VERTICAL

SLIDER
462
401
495
551
lixo_toxico
lixo_toxico
0
15
15.0
1
1
NIL
VERTICAL

TEXTBOX
478
557
527
575
% de lixo
11
0.0
1

SLIDER
299
399
332
549
qtd_alimento
qtd_alimento
5
20
5.0
1
1
NIL
VERTICAL

TEXTBOX
282
557
364
575
% de alimento
11
0.0
1

SLIDER
390
400
423
550
n_depositos
n_depositos
1
10
6.0
1
1
NIL
VERTICAL

TEXTBOX
372
557
449
575
nº de depósitos
11
0.0
1

SLIDER
336
290
508
323
alim_energia
alim_energia
1
50
50.0
1
1
NIL
HORIZONTAL

TEXTBOX
371
337
477
355
Energia por alimento
11
0.0
1

INPUTBOX
1014
212
1169
272
nLimpadores
15.0
1
0
Number

INPUTBOX
1013
281
1168
341
nComiloes
15.0
1
0
Number

TEXTBOX
1053
348
1133
366
nº de agentes
11
0.0
1

INPUTBOX
695
488
850
548
limite_lixo
10.0
1
0
Number

TEXTBOX
665
555
877
583
Limite de lixo a transportar pelos limpadores
11
0.0
1

BUTTON
895
499
958
532
NIL
Go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

PLOT
276
43
544
193
Nº de agentes ao longo da experiência
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -16777216 true "" "plot count comiloes"
"pen-1" 1.0 0 -3844592 true "" "plot count limpadores"
"pen-2" 1.0 0 -14454117 true "" "plot count turtles"
"pen-3" 1.0 0 -12087248 true "" "plot count zombies"

TEXTBOX
278
200
428
256
----- Legenda -----\n> Preto - Comilões\n> Laranja - Limpadores\n> Azul - Total agentes
11
0.0
1

MONITOR
1019
107
1162
152
Nº de limpadores ativos
count limpadores
17
1
11

MONITOR
1026
159
1157
204
Nº de comilões ativos
count comiloes
17
1
11

SWITCH
1033
406
1158
439
reproducao?
reproducao?
1
1
-1000

SWITCH
1034
444
1137
477
zombie?
zombie?
0
1
-1000

MONITOR
1021
48
1113
93
NIL
count zombies
17
1
11

@#$#@#$#@
## Alterações relativas ao Modelo Base

* A simulação agora pára aos 25.000 ticks.
   
* Adicionada existência de zombies:
	* Comilões ao morrerem tornam-se zombies
	* Qualquer agente que se posicionar na mesma célula de um zombie torna-se num
	* Se um zombie tiver algum agente (mortal) à sua volta (num raio de tamanho 1) vira-se para um deles aleatóriamente, se não, move-se aleatóriamente

   
* Adicionada reprodução de comilões
	* Ao atingirem 150 (ou mais) de energia criam uma cópia de si
	* Essa cópia "nasce" com metade da energia do "pai" e anda numa direção diferente do mesmo
	* O "pai" fica com 1/4 da energia que tinha antes de reproduzir

## Do que se trata esta simulação?

A simulação representada pretende simular o comportamento de dois tipos de agentes num mesmo ambiente.

Os agentes aqui representados são: os comilões (representadas por pessoas brancas) e os limpadores (representados por triangulos laranja).

O ambiente contém: lixo normal (células amarelas), lixo tóxico (células vermelhas), depósitos de lixo (células azuis) e alimento (células verdes).

O objetivo dos comilões é sobreviver o máximo possível, ganhando energia quando comem alimento e morrendo quando tocam em lixo.

O objetivo dos limpadores é limpar o ambiente por onde passam. Têm energia que também consome, o que os faz procurar também por alimento. Têm um limite de depósito, sendo que, quando consomem lixo tóxico, ocupam 2 unidades de depósito de lixo aos limpadores, enquanto que o lixo normal ocupa 1. Quando têm o depósito cheio, não podem carregar mais lixo e têm mais dificuldade na obtenção de energia (só recebem cerca de metade da energia por alimento), fazendo que também tenham de encontrar depósitos durante a simulação.

Quando o alimento ou lixos são consumidos, os mesmo reaparecem, mantendo o mesmo número de cada durante a simulação.


## Como começar a simulação?

Para começar a simulação, devem-se configurar:

* Os números de agentes (comilões e limpadores)
* A energia obtida por cada alimento
* A percentagem de alimento presente no ambiente
* O nº de depósitos presentes no ambiente
* A percentagem de cada tipo de lixo presente no ambiente
* E o limite de lixo do depósito de cada limpador

Após a configuração, basta clicar em setup (para dispôr todos os elementos no ambiente na fase inicial) e clicar em go para iniciar a simulação.

A simulação termina quando não houverem agentes vivos ou quando chegar às 50.000 iterações (de forma a não existirem simulações infinitas).

No modelo alterado poderá indicar se deseja ter zombies e/ou reprodução de comilões.


## Realizado Por

Renato Craveiro | 2018011392                    
Introdução À Inteligência Artificial | Lic. Engenharia Informática
Instituto Superior De Engenharia De Coimbra | Ano-Letivo 2021/2022
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.2.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
<experiments>
  <experiment name="repr_5comiloes" repetitions="15" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>count comiloes</metric>
    <enumeratedValueSet variable="nLimpadores">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="reproducao?">
      <value value="true"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="alim_energia">
      <value value="50"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_normal">
      <value value="4"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="zombie?">
      <value value="false"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="n_depositos">
      <value value="6"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="qtd_alimento">
      <value value="13"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="limite_lixo">
      <value value="10"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="nComiloes">
      <value value="5"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_toxico">
      <value value="4"/>
    </enumeratedValueSet>
  </experiment>
  <experiment name="repr_15comiloes" repetitions="15" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>count comiloes</metric>
    <enumeratedValueSet variable="nLimpadores">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="reproducao?">
      <value value="true"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="alim_energia">
      <value value="50"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_normal">
      <value value="4"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="zombie?">
      <value value="false"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="n_depositos">
      <value value="6"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="qtd_alimento">
      <value value="13"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="limite_lixo">
      <value value="10"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="nComiloes">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_toxico">
      <value value="4"/>
    </enumeratedValueSet>
  </experiment>
  <experiment name="zombie_maisalim_menoslixo" repetitions="15" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>count zombies</metric>
    <enumeratedValueSet variable="nLimpadores">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="reproducao?">
      <value value="false"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="alim_energia">
      <value value="50"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_normal">
      <value value="1"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="zombie?">
      <value value="true"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="n_depositos">
      <value value="6"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="qtd_alimento">
      <value value="20"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="limite_lixo">
      <value value="10"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="nComiloes">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_toxico">
      <value value="1"/>
    </enumeratedValueSet>
  </experiment>
  <experiment name="zombie_menosalim_maislixo" repetitions="15" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <metric>count turtles</metric>
    <metric>count zombies</metric>
    <enumeratedValueSet variable="nLimpadores">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="reproducao?">
      <value value="false"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="alim_energia">
      <value value="50"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_normal">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="zombie?">
      <value value="true"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="n_depositos">
      <value value="6"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="qtd_alimento">
      <value value="5"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="limite_lixo">
      <value value="10"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="nComiloes">
      <value value="15"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="lixo_toxico">
      <value value="15"/>
    </enumeratedValueSet>
  </experiment>
</experiments>
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@
