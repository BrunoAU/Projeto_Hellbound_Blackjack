#include "raylib.h"
#include <stdio.h>
#include "hub.h"
#include "selecaoagentes.h"

int guerreiro = 0;
int maga = 1;
int arqueiro = 2;
int parado = 0;
int animado = 1;
int frames = 8;
float frameRate = 12.0f;

typedef struct{
    Texture2D frames[8];
} AnimacaoPersonagem;

void InicializarJogo(void){
    SetWindowTitle("Hellbound Blackjack - Selecao de Personagem");
}

void CarregarAnimacoes(AnimacaoPersonagem* guerreiro, AnimacaoPersonagem* maga, AnimacaoPersonagem* arqueiro){
    char nome[256];
    for(int i=0;i<frames;i++){
        if(i<7) snprintf(nome, sizeof(nome), "assets/Frame_%d_guerreiro.png", i+1);
        else snprintf(nome, sizeof(nome), "assets/Frame_final_guerreiro.png");
        guerreiro->frames[i] = LoadTexture(nome);
        if(i<7) snprintf(nome, sizeof(nome), "assets/Frame_%d_maga.png", i+1);
        else snprintf(nome, sizeof(nome), "assets/Frame_final_maga.png");
        maga->frames[i] = LoadTexture(nome);
        if(i<7) snprintf(nome, sizeof(nome), "assets/Frame_%d_arqueiro.png", i+1);
        else snprintf(nome, sizeof(nome), "assets/Frame_final_arqueiro.png");
        arqueiro->frames[i] = LoadTexture(nome);
    }
}

void AtualizarSelecao(int* estado, int* personagemSel, int* personagemConfirmado, bool* jogoIniciado){
    bool mudou = false;

    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
        (*personagemSel)++;
        if(*personagemSel > arqueiro) *personagemSel = guerreiro;
        mudou = true;
    }
    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
        (*personagemSel)--;
        if(*personagemSel < guerreiro) *personagemSel = arqueiro;
        mudou = true;
    }
    if(mudou){ 
        *estado = animado; 
    }

    if(IsKeyPressed(KEY_ENTER)){
        *personagemConfirmado = *personagemSel;
        *jogoIniciado = true;
    }
}

void AtualizarAnimacao(int* estado, int* frameAtual, float* timer){
    *timer += GetFrameTime();
    if(*timer >= (1.0f/frameRate)){
        *timer = 0.0f;
        (*frameAtual)++;
        if(*frameAtual >= frames){
            *frameAtual = 0;
            *estado = parado;
        }
    }
}

void DesenharCenaEsticada(Texture2D fundo, int estado, int personagemSel, 
    AnimacaoPersonagem guerreiro, AnimacaoPersonagem maga, AnimacaoPersonagem arqueiro, int frameAtual){
    
    Texture2D frameDesenhar = {0};
    if(estado == parado){
        if(personagemSel == 0) frameDesenhar = guerreiro.frames[7];
        else if(personagemSel == 1) frameDesenhar = maga.frames[7];
        else if(personagemSel == 2) frameDesenhar = arqueiro.frames[7];
    }else{
        if(personagemSel == 0) frameDesenhar = guerreiro.frames[frameAtual];
        else if(personagemSel == 1) frameDesenhar = maga.frames[frameAtual];
        else if(personagemSel == 2) frameDesenhar = arqueiro.frames[frameAtual];
    }
    
    Rectangle dest = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Rectangle sourceFundo = { 0.0f, 0.0f, (float)fundo.width, (float)fundo.height };
    Rectangle sourceFrame = { 0.0f, 0.0f, (float)frameDesenhar.width, (float)frameDesenhar.height };

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(fundo, sourceFundo, dest, (Vector2){0,0}, 0.0f, WHITE);
        DrawTexturePro(frameDesenhar, sourceFrame, dest, (Vector2){0,0}, 0.0f, WHITE);
    EndDrawing();
}

void FinalizarJogo(Texture2D fundo, AnimacaoPersonagem guerreiro, AnimacaoPersonagem maga, 
    AnimacaoPersonagem arqueiro){
    UnloadTexture(fundo);
    for(int i=0;i<frames;i++){
        UnloadTexture(guerreiro.frames[i]);
        UnloadTexture(maga.frames[i]);
        UnloadTexture(arqueiro.frames[i]);
    }
}

void MostrarProximaTela(int personagemConfirmado) {
    Personagem p;
    p.dinheiro = 0; 

    if(personagemConfirmado == 0){
        p.nome = "Guerreiro";
        p.coracoes = 5;
        p.coracoes_max = 5; 
    }else if(personagemConfirmado == 1){
        p.nome = "Maga";
        p.coracoes = 3;
        p.coracoes_max = 3;
    }else{
        p.nome = "Arqueiro";
        p.coracoes = 4;
        p.coracoes_max = 4; 
    }
    TelaHub(p);
}

int RodarTelaSelecao(void){
    InicializarJogo(); 

    Texture2D fundo = LoadTexture("assets/Fundo.png");
    
    AnimacaoPersonagem animGuerreiro, animMaga, animArqueiro;
    CarregarAnimacoes(&animGuerreiro, &animMaga, &animArqueiro);

    int estado = parado;
    int personagemSel = guerreiro;
    int frameAnimAtual = 0;
    float timerAnim = 0.0f;
    bool jogoIniciado = false;
    int personagemConfirmado = guerreiro;

    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("CARREGANDO...", 10, 10, 20, GRAY);
        EndDrawing();
    }

    while(!WindowShouldClose() && !jogoIniciado){
        
        AtualizarSelecao(&estado, &personagemSel, &personagemConfirmado, &jogoIniciado);
        
        if (estado == animado) {
            AtualizarAnimacao(&estado, &frameAnimAtual, &timerAnim);
        }
        
        DesenharCenaEsticada(fundo, estado, personagemSel, animGuerreiro, animMaga, animArqueiro, frameAnimAtual);
    }

    FinalizarJogo(fundo, animGuerreiro, animMaga, animArqueiro);
    
    if(jogoIniciado){
        MostrarProximaTela(personagemConfirmado);
    }
    
    return 0;
}
