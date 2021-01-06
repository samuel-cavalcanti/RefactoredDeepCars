# Refactored Deep Cars

Tentando limpar o código de Deep Cars do universo programado  
[AI learning to DRIVE!!](https://www.youtube.com/watch?v=gnfkfUQvKDw&ab_channel=UniversoProgramado)  

[![Build Status](https://travis-ci.org/samuel-cavalcanti/RefactoredDeepCars.svg?branch=main)](https://travis-ci.org/samuel-cavalcanti/RefactoredDeepCars)

![](preview.gif)

Até o momento:
- foi criado um arquivo CMakeList.txt que lista
  as dependências do projeto
  
- Reestruturado em Classes

- adicionado GTest (C++ test framework da google)


- Precisa ser feito: 

- criar testes de unidade para cada Classe (:sob:)
 

## dependências

- SDL2
- SDL2_image
- SDL2_ttf
- GSL
- GTest

certifique-se de instalar corretamente cada dependência no
seu sistema operacional favorito.  
No meu caso utilizo Arch linux logo os comandos são:
```shell
sudo pacman -Su sdl2 sdl2_image sdl2_ttf gsl gtest
```
No Ubuntu Bionic (18.04 LTS)
```shell
sudo apt install libsdl2-ttf-dev libsdl2-image-dev
libsdl2-dev libgtest-dev libgsl-dev
```

# executando o projeto
Uma vez tendo as dependências instaladas você pode:
- executar os testes
- executar o jogo e ver a rede neural aprendendo.

para isso você precisa, baixar e compilar o projeto:
no linux:
```shell
mkdir build
cd build 
cmake ..
make -j2
```
Caso queria ver a rede neural aprendendo:
```shell
./DeepCars ../fontes/arial.ttf ../imagens/ ../matrizDistancias
```
Caso queria executar os testes:
```shell
./TestDeepCars
```
