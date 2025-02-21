# 3D RayCaster

Este projeto é um simples motor de raycasting 3D implementado em C usando SDL2. Ele demonstra técnicas básicas de renderização 3D simulando uma perspectiva em primeira pessoa em um mundo baseado em grade 2D.

## Funcionalidades

- Renderização 3D básica usando raycasting
- Movimento do jogador com detecção de colisão
- Representação simples do mapa

## Requisitos

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer

## Configuração

1. Instale as bibliotecas necessárias.
2. Clone este repositório:
    ```sh
    git clone https://github.com/seuusuario/3D-rayCaster.git
    ```
3. Navegue até o diretório do projeto:
    ```sh
    cd 3D-rayCaster
    ```
4. Compile o projeto:
    ```sh
    gcc -o raycaster main.c src/control.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
    ```

## Uso

Execute o executável compilado:
```sh
./raycaster
```

Use as teclas de seta para mover o jogador:
- Cima: Mover para frente
- Baixo: Mover para trás
- Esquerda: Virar à esquerda
- Direita: Virar à direita

## Licença

Este projeto está licenciado sob a Licença MIT.