# Hunter adventures
## Jogo de plataforma programado em C++

### Desenvolvedores:
* Moises de Paulo Dias (Programação)
* [Ian Douglas Almeida Queros](https://github.com/ianqueros) (Programação e ilustração do jogo)

### Descrição:
Hunter Adventures é um jogo de plataforma (side scrolling), criado utilizando a linguagem C++ e a biblioteca gráfica Allegro. O jogo foi feito em 2017, para a disciplina de Técnicas de programação do curso de engenharia de computação da UTFPR, onde foi ensinado programação orientada a objetos e proposto um projeto relacionado ao tema para os alunos. Foram utilizadas trilhas sonoras dos jogos Final fantasy, Castlevania e Dark Souls, toda a programação foi feita pelos dois integrantes do grupo e todos os desenhos foram criados pelo Ian Douglas durante o desenvolvimento do jogo.

#### Clique na imagem para visualizar um vídeo de alguns minutos de jogo:

[![hunter](https://img.youtube.com/vi/Pikd5Le-Xis/0.jpg)](https://www.youtube.com/watch?v=Pikd5Le-Xis "gameplay hunter adventures")

### O jogo possui as seguintes funcionalidades:
* Ranking de pontos
* Opção para salvar o jogo
* Opção de um ou dois jogadores
* Opção de jogar pelo teclado ou joystick
* Três fases e mais de dez inimigos diferentes

### Comandos:
#### Menu e Pause:
| Comando     | Acão   |
| ----------- |:------:|
| Navegação   | Setas  |
| Selecionar  | X      |
| Retornar    | C      |

#### Jogo:
| Comando       | Jogador 1  | Jogador 2  |
| ------------- |:----------:|:----------:|
| Movimentação  | Setas      | I-J-K-L    |
| Ataque        | Z          | A          |
| Pulo          | X          | S          |
| Arrancada     | C          | D          |
| Pausar        | V          | V          |

### Dicas:
* Além do ataque horizontal, você pode executar um ataque vertical segurando o botão direcional para cima e atacando.
* Após pular você pode combinar ataques e arrancadas para alcançar distâncias maiores.
* Durante uma arrancada você fica imune a ataques.

### Instruções para execução ou compilação do jogo:
#### Para jogar basta executar o arquivo HUNTER ADVENTURES.exe na pasta "/executavel"

#### Para compilar o jogo siga as instruções:
1. Baixe e instale a IDE [Code::Blocks 16.01](http://www.codeblocks.org/downloads/5) e a pasta [Allegro](https://www.dropbox.com/sh/bslxa5zsztczt17/AACu7tK-yixpoitT9NGg-xXZa?dl=0)
2. Criar um novo projeto vazio no C::B
3. Passar os arquivos existentes na pasta "/executavel" (menos o .exe) e a pasta "/arquivos .h e .cpp" para a pasta do projeto criado.
4. No C::B ir em Project->Build options...
5. Dentro do Build options... ir na aba Compiler Flags e marcar a opção "Have g++ follow the C++11 ISO C++ language standard [-std=c++11]"
6. Ir na aba Linker settings e abaixo de Link libraries clicar no botão Add e adicionar "...\allegro\lib\liballegro-5.0.10-monolith-mt.a" (sem aspas e "..." representa a pasta em que foi salvo a pasta /allegro).
7. Ainda no Build options... ir na aba Search directories->Compiler, clicar no botão Add e adicionar "...\allegro\include" (sem aspas).
8. Ainda no Build options... ir na aba Search directories->Linker, clicar no botão Add e adicionar "...\allegro\lib" (sem aspas).

**Observação: Este software foi feito utilizando o programa Code::Blocks 16.01 e, portanto, essas instruções são refentes a ele.**
